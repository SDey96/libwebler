#include <iostream>
#include <webler/dm.hpp>

int main() {
  web::DownloadFilePartitions DFP;
  string url;
  cin>>url;
  string filepath = "~/Desktop/out.txt";
  DFP.UserInterface(url,filepath);
  return 0;
}