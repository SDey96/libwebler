#include <downloader/dm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include<cmath>
#include <vector>
#include <iterator>
#include <algorithm>
#include <curl/curl.h>
#define MAX_NO_OF_THREADS 10

using namespace std;

web::DownloadFilePartitions::DownloadFilePartitions (){           //Constructor for the class.
  fullFileSize = 0.0;
  fileSizeResult = 0;
  for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
    tempFileNames[i] = tempName + to_string(i+1);                 //OPening MAX_NO_OF_THREADS temporary files.
  }
};

auto web::DownloadFilePartitions::FileSize(const char *fileUrl) -> int {
  CURL *tempCurl = curl_easy_init();                              //Initiate tempCurl to get file size.
  CURLcode tempCode;
  if(tempCurl) {
    curl_easy_setopt(tempCurl, CURLOPT_URL, fileUrl);             //Send the url request
    curl_easy_setopt(tempCurl, CURLOPT_NOBODY, 1000L);
    tempCode = curl_easy_perform(tempCurl);
    if(CURLE_OK == tempCode) {
      tempCode = curl_easy_getinfo(tempCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,&fullFileSize);
      if(CURLE_OK == tempCode && fullFileSize > 0.0){             //Get the file type and download.
        tempCode = curl_easy_getinfo(tempCurl, CURLINFO_CONTENT_TYPE, &contentType);
        extension = string(contentType);
        curl_easy_cleanup(tempCurl);
        return fullFileSize;
      }
    }
  }
  return -1;
}

auto web::DownloadFilePartitions::FileRanges() -> void {           //Function to split file into multiple parts.
  int tempVar = fileSizeResult;
  int remainderSize = fileSizeResult%MAX_NO_OF_THREADS;
  string tempString;
  int division1 = 0;
  int division2 = 0;
  for (size_t i = 0; i < MAX_NO_OF_THREADS;i++) {
    division1 = (tempVar*i)/MAX_NO_OF_THREADS;                     //Divide into two temp variables.
    division2 = (tempVar*(i+1))/MAX_NO_OF_THREADS;
    if(i>0){
      division1++;
    }
    tempString = to_string(division1);
    tempString.append("-");
    tempString.append(to_string(division2));
    fileRanges.push_back(tempString);
  }
}

auto web::DownloadFilePartitions::getSizeToEnd(ifstream& is) -> streampos {
    auto currentPosition = is.tellg();                               //Function to write into buffer and read from the buffer.
    is.seekg(0, is.end);
    auto length = is.tellg() - currentPosition;
    is.seekg(currentPosition, is.beg);
    return length;
}

auto web::DownloadFilePartitions::MergeDownloadedPartitions() -> void {
    ofstream mainFileOutputStream;
    mainFileOutputStream.open(oFilePath.c_str(),ios::binary|ios::out);
    for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {                 //We will write data from the temporary files into our main files.
      ifstream tempInputStream;
      tempInputStream.open(tempFileNames[i],ios::binary|ios::in);
      vector<char> buff(web::DownloadFilePartitions::getSizeToEnd(tempInputStream),0);
      tempInputStream.read(buff.data(),buff.size());                 //Write into the buff
      mainFileOutputStream.write(buff.data(),buff.size());           //Read from buff and write into main output file.
    }
}

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata){
    std::ofstream *out = static_cast<std::ofstream *>(userdata);
    size_t nbytes = size * nmemb;
    out->write(ptr, nbytes);                                          //Specify the sie data to write to.
    return nbytes;                                                    //Return the bytes.

}

int progress_func(void* ptr, double TotalToDownload, double NowDownloaded,double TotalToUpload, double NowUploaded) {
    if (TotalToDownload <= 0.0) {                                     // Ensure that the file to be downloaded is not empty.
        return 0;                                                     // Because that would cause a division by zero error.
    }
    double fractiondownloaded = NowDownloaded / TotalToDownload;
    printf("Download Progress [%3.0f%%]",fractiondownloaded*100);     //Using printf since it is atomic.
    printf("\r");                                                     // And back to the beginning of the line!
    fflush(stdout);
    return 0;                                                         // We should return zero for the complete transaciton.
}

auto web::DownloadFilePartitions::WriteTemporaryPartitions(int index) -> void {
  CURL *curl = curl_easy_init();                                      //Initiate curl.
  curl_easy_setopt(curl, CURLOPT_URL,finalUrl.c_str());               //Pass the url to download the file from.
  curl_easy_setopt(curl, CURLOPT_RANGE,fileRanges[index].c_str());    //Set the ranges to download.
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tempOutputFiles[index]); //Pass the stream pointers.
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS,0);                       //Initiate curl with NOPROGRESS with 0.
  curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);    //Pass the callback function which will calculate the downlaod progress.
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_data);           //Pass the callback function which will write the files.
  CURLcode tempResult = curl_easy_perform(curl);
  return;
}

auto web::DownloadFilePartitions::DownloadFile (const char *url, const char *outFile) -> int {
  std::ofstream output(outFile, ios::binary);                         //Open the output stream.
  fileSizeResult = FileSize(url);                                     //Determine the size of file.
  FileRanges();                                                       //Split the file into partitions of equal size.
  if (fileSizeResult > 0) {
      for (int i = 0; i < MAX_NO_OF_THREADS; i++) {
        fileThreadsToDownload[i] = thread(&DownloadFilePartitions::WriteTemporaryPartitions,this,i);
      }                                                               //Call the function multithreaded.
      for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
        fileThreadsToDownload[i].join();                              //Wait for all the threads to complete executing.
        tempOutputFiles[i].close();                                   //Close all the temporary files.
      }
      cout<<endl;
      MergeDownloadedPartitions();                                    //Now merge all the downaloaded parts to form a single file.
      return 0;
  }
  else{
      return -1;
  }
}

auto web::DownloadFilePartitions::DetermineFileExtension() -> void { //Determine the extension of the downloaded file.
   if(extension.find("text") != string::npos){
      extension = ".txt";
   }
   else if(extension.find("pdf") != string::npos){
     extension = ".pdf";
   }
   else if(extension.find("png") != string::npos){
     extension = ".png";
   }
   else if(extension.find("jpg") != string::npos){
     extension = ".jpg";
   }
   else if(extension.find("jpeg") != string::npos){
     extension = ".jpeg";
   }
   else if(extension.find("mp4") != string::npos){
     extension = ".mp4";
   }
   else if(extension.find("iso") != string::npos){
     extension = ".iso";
   }
   else if(extension.find("zip") != string::npos){
     extension = ".zip";
   }
   else if(extension.find("7z") != string::npos){
     extension = ".7z";
   }
   else if(extension.find("deb") != string::npos){
     extension = ".deb";
   }
   else if(extension.find("pkg") != string::npos){
     extension = ".pkg";
   }
   else if(extension.find("rar") != string::npos){
     extension = ".rar";
   }
   else if(extension.find("tar.gz") != string::npos){
     extension = ".tar.gz";
   }
   else if(extension.find("rar") != string::npos){
     extension = ".rar";
   }
   else if(extension.find("bin") != string::npos){
     extension = ".bin";
   }
   else if(extension.find("dmg") != string::npos){
     extension = ".dmg  ";
   }
   else if(extension.find("db") != string::npos){
     extension = ".db";
   }
   else if(extension.find("log") != string::npos){
     extension = ".log";
   }
   else if(extension.find("sql") != string::npos){
     extension = ".sql";
   }
   else if(extension.find("tar") != string::npos){
     extension = ".tar";
   }
   else if(extension.find("xml") != string::npos){
     extension = ".xml";
   }
   else if(extension.find("jar") != string::npos){
     extension = ".jar";
   }
   else if(extension.find("svg") != string::npos){
     extension = ".svg";
   }
   else if(extension.find("php") != string::npos){
     extension = ".php";
   }
   else if(extension.find("ppt") != string::npos){
     extension = ".ppt";
   }
   else if(extension.find("pptx") != string::npos){
     extension = ".pptx";
   }
   else if(extension.find("pps") != string::npos){
     extension = ".pps";
   }
   else if(extension.find("xlsx") != string::npos){
     extension = ".xlsx";
   }
   else if(extension.find("sys") != string::npos){
     extension = ".sys";
   }
   else if(extension.find("sys") != string::npos){
     extension = ".sys";
   }
   else if(extension.find("tmp") != string::npos){
     extension = ".tmp";
   }
   else if(extension.find("doc") != string::npos){
     extension = ".doc";
   }
   else if(extension.find("docx") != string::npos){
     extension = ".docx";
   }
   string temp = oFilePath + extension;
   rename(oFilePath.c_str(),temp.c_str());                            //Rename the file.
}

auto web::DownloadFilePartitions::UserInput(string url, string outFile) -> void {
  finalUrl = url;
  oFilePath = outFile;
  cout<<"Downloading..."<<endl;
  for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
    tempOutputFiles[i].open(tempFileNames[i]);                        //Open the temp files.
  }
  int fileResult = DownloadFile(url.c_str(),outFile.c_str());
  if (fileResult ==  0){
        DetermineFileExtension();
        cout <<"Download successful.\n" << endl;
  }else{
        cout<<"Error in downloading.\n"<<endl;
  }
  for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {                   //Remove the temporary files.
       remove(tempFileNames[i].c_str());
  }
}
