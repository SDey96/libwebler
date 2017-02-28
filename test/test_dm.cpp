#include <iostream>
#include <webler/dm.hpp>

int main() {
  web::DownloadFilePartitions DFP;
  string url;
  cin>>url;

  string filepath,filename;
  cin>>filename>>filepath;
  filepath = filepath + filename;
  
  DFP.UserInterface(url,filepath);
  
  return 0;
}
