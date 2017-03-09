#include <downloader/Downloader.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <cmath>
#include <vector>
#include <iterator>
#include <algorithm>
#include <curl/curl.h>
#include <pwd.h>
#include <ctime>
#include <unistd.h>

#ifndef MAX_NO_OF_THREADS
#define MAX_NO_OF_THREADS 10
#endif

using namespace std;

webler::Downloader::Downloader (){                                              //Constructor for the class.
  fullFileSize = 0.0;
  fileSizeResult = 0;
  progressCallback = NULL;
  if (getenv("HOME") == NULL) {
    downloaddir = getpwuid(getuid())->pw_dir;
  } else {
    downloaddir = getenv("HOME");
  }
  downloaddir += "/Downloads";
  tempName = GetRandomString(6);
  for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
    tempFileNames[i] = downloaddir + "/" + tempName + to_string(i+1);           //Opening MAX_NO_OF_THREADS temporary files.
  }
};

auto webler::Downloader::GetRandomString(const int len) -> string {
  string s = "";
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  for (int i = 0; i < len; ++i) {
      s += alphanum[(rand()+clock()) % (sizeof(alphanum) - 1)];
  }
  return s;
}

auto webler::Downloader::SetProgressCallback(void (*callback)(double)) -> void {
  progressCallback = callback;                                                  //Set the callback
}

auto webler::Downloader::FileSize(const char *fileUrl) -> int {
  CURL *tempCurl = curl_easy_init();                                            //Initiate curl handle to get file size.
  CURLcode tempCode;                                                            //CURLcode for analyzong the result.
  if(tempCurl) {
    curl_easy_setopt(tempCurl, CURLOPT_URL, fileUrl);                           //Send the url request
    curl_easy_setopt(tempCurl, CURLOPT_NOBODY, 1L);
    tempCode = curl_easy_perform(tempCurl);
    if(CURLE_OK == tempCode) {
      tempCode = curl_easy_getinfo(tempCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,&fullFileSize);
      if(CURLE_OK == tempCode && fullFileSize > 0.0){                           //Get the file type and download.
        tempCode = curl_easy_getinfo(tempCurl, CURLINFO_CONTENT_TYPE, &contentType);
        extension = string(contentType);                                        //Set the extension.
        curl_easy_cleanup(tempCurl);                                            //Closes all the connections to the url.
        return fullFileSize;                                                    //Return file size upon success.
      }
    }
  }
  return -1;                                                                    //Return -1 upon failure.
}

auto webler::Downloader::FileRanges() -> void {                                 //Function to split file into multiple parts.
  int tempVar = fileSizeResult;
  int remainderSize = fileSizeResult%MAX_NO_OF_THREADS;
  string tempString;
  int division1 = 0;
  int division2 = 0;
  for (size_t i = 0; i < MAX_NO_OF_THREADS;i++) {
    division1 = (tempVar*i)/MAX_NO_OF_THREADS;                                  //Divide into two temp variables.
    division2 = (tempVar*(i+1))/MAX_NO_OF_THREADS;
    if(i>0){
      division1++;
    }
    tempString = to_string(division1);
    tempString.append("-");
    tempString.append(to_string(division2));
    fileRanges.push_back(tempString);                                           //Insert into the vector.
  }
}

auto webler::Downloader::getSizeToEnd(ifstream& is) -> streampos {
    auto currentPosition = is.tellg();                                          //Function to write into buffer and read from the buffer.
    is.seekg(0, is.end);
    auto length = is.tellg() - currentPosition;
    is.seekg(currentPosition, is.beg);
    return length;
}

auto webler::Downloader::WriteTemporaryPartitions(int index) -> void {
  CURL *curl = curl_easy_init();                                                //Initiate curl.
  curl_easy_setopt(curl, CURLOPT_URL,finalUrl.c_str());                         //Pass the url to download the file from.
  curl_easy_setopt(curl, CURLOPT_RANGE,fileRanges[index].c_str());              //Set the ranges to download.
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tempOutputFiles[index]);           //Pass the stream pointers.
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_data);                     //Pass the callback function which will write the files.
  CURLcode tempResult = curl_easy_perform(curl);                                //Close all connections after completing the url write back.
  return;
}

auto webler::Downloader::DownloadFile (const char *url, const char *outFile) -> int {
  std::ofstream output(outFile, ios::binary);                                   //Open the output stream.
  fileSizeResult = FileSize(url);                                               //Determine the size of file.
  FileRanges();                                                                 //Split the file into partitions of equal size.
  if (fileSizeResult > 0) {
      thread progressThread = thread(&Downloader::getDownloadProgress,this);    //Thread to calculate the progress.
      for (int i = 0; i < MAX_NO_OF_THREADS; i++) {
        fileThreadsToDownload[i] = thread(&Downloader::WriteTemporaryPartitions,this,i);
      }                                                                         //Call the function multithreaded.
      for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
        fileThreadsToDownload[i].join();                                        //Wait for all the threads to complete executing.
        tempOutputFiles[i].close();                                             //Close all the temporary files.
      }
      progressThread.join();                                                    //Wait for progress function to complete its callback
      cout<<endl;
      MergeDownloadedPartitions();                                              //Now merge all the downaloaded parts to form a single file.
      return 0;
  }
  else{
      return -1;
  }
}

auto webler::Downloader::MergeDownloadedPartitions() -> void {
    ofstream mainFileOutputStream;
    mainFileOutputStream.open(oFilePath.c_str(),ios::binary|ios::out);
    for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {                            //We will write data from the temporary files into our main files.
      ifstream tempInputStream;
      tempInputStream.open(tempFileNames[i],ios::binary|ios::in);
      vector<char> buff(webler::Downloader::getSizeToEnd(tempInputStream),0);
      tempInputStream.read(buff.data(),buff.size());                            //Write into the buff
      mainFileOutputStream.write(buff.data(),buff.size());                      //Read from buff and write into main output file.
    }
}

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata){
    std::ofstream *out = static_cast<std::ofstream *>(userdata);
    size_t nbytes = size * nmemb;
    out->write(ptr, nbytes);                                                    //Specify the sie data to write to.
    return nbytes;                                                              //Return the bytes.

}

auto webler::Downloader::getDownloadProgress() -> void {
    if(progressCallback) {
      ifstream *files = new ifstream[MAX_NO_OF_THREADS];
      int indFileSize = 0;
      double percentComplete = 0;
      while(indFileSize!=fileSizeResult){
        indFileSize = 0;
        for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {                        //Calculate the size of all the files.
            ifstream file(tempFileNames[i].c_str(),ifstream::binary|ifstream::ate);
            indFileSize += file.tellg();
        }
        percentComplete = ((double)indFileSize/(double)fileSizeResult)*100;
        progressCallback(percentComplete);
        usleep(10000);
     }
  }
}

auto webler::Downloader::DetermineFileExtension() -> void {                     //Determine the extension of the downloaded file.
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
     extension = ".gif";
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
   else if(extension.find("x-7z-compressed") != string::npos){
     extension = ".7z";
   }
   else if(extension.find("deb") != string::npos){
     extension = ".deb";
   }
   else if(extension.find("pkg") != string::npos){
     extension = ".pkg";
   }
   else if(extension.find("x-rar-compressed") != string::npos){
     extension = ".rar";
   }
   else if(extension.find("gzip") != string::npos){
     extension = ".tar.gz";
   }
   else if(extension.find("octet-stream") != string::npos){
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
   else if(extension.find("x-tar") != string::npos){
     extension = ".tar";
   }
   else if(extension.find("html") != string::npos){
     extension = ".html";
   }
   else if(extension.find("css") != string::npos){
     extension = ".css";
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
   else if(extension.find("sys") != string::npos){
     extension = ".sys";
   }
   else if(extension.find("tmp") != string::npos){
     extension = ".tmp";
   }
   else if(extension.find("msword") != string::npos){
     extension = ".doc";
   }
   else if(extension.find("mspowerpoint") != string::npos){
     extension = ".pps";
   }
   else if(extension.find("vnd.ms-powerpoint") != string::npos){
     extension = ".ppt";
   }
   else if(extension.find("vnd.openxmlformats-officedocument.presentationml.presentation") != string::npos){
     extension = ".pptx";
   }
   else if(extension.find("vnd.openxmlformats-officedocument.spreadsheetml.sheet") != string::npos){
     extension = ".xlsx";
   }
   else if(extension.find("vnd.openxmlformats-officedocument.wordprocessingml.document") != string::npos){
     extension = ".docx";
   }
   string temp = oFilePath + extension;
   rename(oFilePath.c_str(),temp.c_str());                                      //Rename the file.
}

auto webler::Downloader::download(string url, string outFile) -> bool {

  finalUrl = url;
  if(outFile==""){
    outFile = GetRandomString(8);                                               //Give a random name if the file was not given one by the user.
  }
  oFilePath = downloaddir + "/" + outFile;                                      //Create and open the file path.

  for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
    tempOutputFiles[i].open(tempFileNames[i]);                                  //Open the temp files.
  }

  int fileResult = DownloadFile(url.c_str(),oFilePath.c_str());
  for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {                              //Remove the temporary files after the download.
       remove(tempFileNames[i].c_str());
  }
  if (fileResult ==  0){
        DetermineFileExtension();
        return true;
  }else{
        return false;
  }
}

#undef MAX_NO_OF_THREADS
