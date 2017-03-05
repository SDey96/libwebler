#include <iostream>
#include <webler/Downloader.hpp>

int main() {
  webler::Downloader DFP;
  string url;
  cin>>url;

  string filename;
  cin>>filename;
  cout << "Downloading ..." << endl;
  if( DFP.download(url,filename) ) {
  	cout << "Download successful" << endl;
  } else {
  	cout << "Failed download" << endl;
  }
  return 0;
}
