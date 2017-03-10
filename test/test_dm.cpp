#include <iostream>
#include <webler/Downloader.hpp>
#include <stdio.h>

void progressCallback(double percentage) {
  printf("Download Progress : %.3f",percentage);
  printf("\r");
}

int main() {

  webler::Downloader my_downloader;
  string url;
  cout << "Enter download URL:" << endl;
  cin>>url;
  string filename;
  cout << endl << "Enter file name:" << endl;
  cin>>filename;
  my_downloader.SetProgressCallback(progressCallback);
  cout << endl << "Downloading ..." << endl;
  if( my_downloader.download(url,filename) ) {
  	cout << endl << "Download successful" << endl;
  } else {
  	cout << endl << "Download failed" << endl;
  }
  return 0;
}
