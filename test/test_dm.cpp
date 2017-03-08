#include <iostream>
#include <webler/Downloader.hpp>
#include <stdio.h>

void progressCallback(double percentage) {
  printf("Download Progress : %.3f",percentage);
  printf("\r");
}

int main() {
  webler::Downloader DFP;
  string url;
  cin>>url;

  string filename;
  cin>>filename;
  DFP.SetProgressCallback(progressCallback);
  cout << "Downloading ..." << endl;
  if( DFP.download(url,filename) ) {
  	cout << "Download successful" << endl;
  } else {
  	cout << "Failed download" << endl;
  }
  return 0;
}
