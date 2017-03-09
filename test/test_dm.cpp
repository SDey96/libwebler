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
  cin>>url;
  string filename;
  cin>>filename;
  my_downloader.SetProgressCallback(progressCallback);
  cout << "Downloading ..." << endl;
  if( my_downloader.download(url,filename) ) {
  	cout << "Download successful" << endl;
  } else {
  	cout << "Failed download" << endl;
  }
  return 0;
}
