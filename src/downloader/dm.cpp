#include <downloader/dm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <iterator>
#include <algorithm>
#include <curl/curl.h>
#define MAX_NO_OF_THREADS 5

using namespace std;

web::DownloadFilePartitions::DownloadFilePartitions (){
  fullFileSize = 0.0;
  fileSizeResult = 0;
  for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
    tempFileNames[i] = tempName + to_string(i+1);
    tempOutputFiles[i].open(tempFileNames[i]);
  }
};

auto web::DownloadFilePartitions::FileSize(const char *fileUrl) -> int {
  CURL *tempCurl = curl_easy_init();
  CURLcode tempCode;
  if(tempCurl) {
    curl_easy_setopt(tempCurl, CURLOPT_URL, fileUrl);
    curl_easy_setopt(tempCurl, CURLOPT_NOBODY, 1L);
    tempCode = curl_easy_perform(tempCurl);
    if(CURLE_OK == tempCode) {
      tempCode = curl_easy_getinfo(tempCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,&fullFileSize);
      if(CURLE_OK == tempCode && fullFileSize > 0.0){
        tempCode = curl_easy_getinfo(tempCurl, CURLINFO_CONTENT_TYPE, &contentType);
        //cout<<contentType<<endl;
        extension = string(contentType);
        curl_easy_cleanup(tempCurl);
        return fullFileSize;
      }
    }
  }
  return -1;
}

auto web::DownloadFilePartitions::FileRanges() -> void {
  int tempVar = fileSizeResult;
  int remainderSize = fileSizeResult%MAX_NO_OF_THREADS;
  string tempString;
  int division1 = 0;
  int division2 = 0;
  for (size_t i = 0; i < MAX_NO_OF_THREADS;i++) {
    division1 = (tempVar*i)/MAX_NO_OF_THREADS;
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

streampos getSizeToEnd(ifstream& is)
{
    auto currentPosition = is.tellg();
    is.seekg(0, is.end);
    auto length = is.tellg() - currentPosition;
    is.seekg(currentPosition, is.beg);
    return length;
}

auto web::DownloadFilePartitions::MergeDownloadedPartitions() -> void {
    ofstream mainFileOutputStream;
    mainFileOutputStream.open(oFilePath.c_str(),ios::binary|ios::out);
    for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
      ifstream tempInputStream;
      tempInputStream.open(tempFileNames[i],ios::binary|ios::in);
      vector<char> buff(getSizeToEnd(tempInputStream),0);
      tempInputStream.read(buff.data(),buff.size());
      mainFileOutputStream.write(buff.data(),buff.size());
    }
}

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    //cout  << " " << size << " " << nmemb << endl;
    std::ofstream *out = static_cast<std::ofstream *>(userdata);
    size_t nbytes = size * nmemb;
    out->write(ptr, nbytes);
    return nbytes;
}

auto web::DownloadFilePartitions::WriteTemporaryPartitions(int index) -> void {
  CURL *curl = curl_easy_init();
  // cout<<finalUrl.c_str();
  curl_easy_setopt(curl, CURLOPT_URL,finalUrl.c_str());
  // cout<<fileRanges[index] <<endl;
  curl_easy_setopt(curl, CURLOPT_RANGE,fileRanges[index].c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tempOutputFiles[index]);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_data);
  CURLcode tempResult = curl_easy_perform(curl);
  return;
}

auto web::DownloadFilePartitions::DownloadFile (const char *url, const char *outFile) -> int {
  std::ofstream output(outFile, ios::binary);
  fileSizeResult = FileSize(url);
  FileRanges();
  if (fileSizeResult > 0) {
      for (int i = 0; i < MAX_NO_OF_THREADS; i++) {
        fileThreadsToDownload[i] = thread(&DownloadFilePartitions::WriteTemporaryPartitions,this,i);
      }
      for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
        fileThreadsToDownload[i].join();
        tempOutputFiles[i].close();
      }
      MergeDownloadedPartitions();
      return 0;
  }
  else{
      return -1;
  }
}

auto web::DownloadFilePartitions::CleanUpTempFiles() -> void {
   //cout<<"The extension is : "<<extension<<endl;
   if(extension.find("text") != string::npos){
      extension = ".text";
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
   //cout<<extension<<endl;
   string temp = "result" + extension;
   //cout<<temp<<endl;
   rename(oFilePath.c_str(),temp.c_str());
   for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
        remove(tempFileNames[i].c_str());
   }
}

auto web::DownloadFilePartitions::UserInterface(string url, string outFile) -> void {
  oFilePath = outFile;
  finalUrl = url;
  cout<<"Downloading..."<<endl;
  int fileResult = DownloadFile(url.c_str(),outFile.c_str());
  if (fileResult ==  0){
        CleanUpTempFiles();
        cout <<"Download successful.\n" << endl;
  }else{
        cout<<"Error in downloading.\n"<<endl;
  }
}
