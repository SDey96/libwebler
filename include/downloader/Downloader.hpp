#ifndef __WEBLER_DOWNLOADER__

#define __WEBLER_DOWNLOADER__

#include <fstream>
#include <cstring>
#include <thread>
#include <mutex>
#include <cmath>
#include <ctime>
#include <vector>
#include <iterator>
#include <algorithm>
#include <curl/curl.h>

#ifndef MAX_NO_OF_THREADS
#define MAX_NO_OF_THREADS 10
#endif

using namespace std;

namespace webler {


  class Downloader
  {
    private:
      string extension;
      string oFilePath;
      string finalUrl;
      char *contentType;
      ofstream *outPipe;
      int fileSizeResult;
      double fullFileSize;
      vector<string> fileRanges;
      string tempName;
      string tempFileNames[MAX_NO_OF_THREADS];
      ofstream tempOutputFiles[MAX_NO_OF_THREADS];
      thread fileThreadsToDownload[MAX_NO_OF_THREADS];
      string downloaddir;

      auto FileRanges() -> void;
      auto DetermineFileType() -> void;
      auto DetermineFileExtension()  -> void;
      auto FileSize(const char *url) -> int;
      auto MergeDownloadedPartitions() -> void;
      auto getSizeToEnd(ifstream& is) -> streampos;
      auto WriteTemporaryPartitions(int index) -> void;
      auto DownloadFile(const char *url, const char *outFile) -> int;

    public:
      Downloader();
      auto download(string url, string outFile="") -> bool;

  };

}

#endif
