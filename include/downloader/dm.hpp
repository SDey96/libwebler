#ifndef __WEBLER_DOWNLOADER__

#define __WEBLER_DOWNLOADER__

#include <fstream>
#include <cstring>
#include <thread>
#include <mutex>
#include <cmath>
#include <vector>
#include <iterator>
#include <algorithm>
#include <curl/curl.h>

#define MAX_NO_OF_THREADS 10
using namespace std;

namespace web{

  class DownloadFilePartitions
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
      string tempName = "tempOutput";
      string tempFileNames[MAX_NO_OF_THREADS];
      ofstream tempOutputFiles[MAX_NO_OF_THREADS];
      thread fileThreadsToDownload[MAX_NO_OF_THREADS];

      auto FileRanges() -> void;
      auto DetermineFileType() -> void;
      auto DetermineFileExtension()  -> void;
      auto FileSize(const char *url) -> int;
      auto MergeDownloadedPartitions() -> void;
      auto getSizeToEnd(ifstream& is) -> streampos;
      auto WriteTemporaryPartitions(int index) -> void;

    public:
      DownloadFilePartitions();
      auto UserInput(string url, string outFile) -> void;
      auto DownloadFile(const char *url, const char *outFile) -> int;

  };

}

#endif
