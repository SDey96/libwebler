#include <fstream>
#include <cstring>
#include <thread>
#include <mutex>
#include <vector>
#include <iterator>
#include <algorithm>
#include <curl/curl.h>
#define MAX_NO_OF_THREADS 5
using namespace std;

namespace web{
  class DownloadFilePartitions
  {
    private:
      char *contentType;
      ofstream *outPipe;
      CURLcode fileResult;
      string extension;
      CURLcode fileSize;
      double fullFileSize;
      string oFilePath;
      int fileSizeResult;
      string finalUrl;
      vector<string> fileRanges;
      mutex mtx;
      string tempName = "tempOutput";
      string tempFileNames[MAX_NO_OF_THREADS];
      ofstream tempOutputFiles[MAX_NO_OF_THREADS];
      thread fileThreadsToDownload[MAX_NO_OF_THREADS];
    public:
      DownloadFilePartitions();
      auto FileRanges() -> void;
      auto DetermineFileType() -> void;
      auto CleanUpTempFiles()  -> void;
      auto FileSize(const char *url) -> int;
      auto WriteTemporaryPartitions(int index) -> void;
      auto UserInterface(string url, string outFile) -> void;
      auto DownloadFile(const char *url, const char *outFile) -> int;
      auto MergeDownloadedPartitions() -> void;
      auto ProcessData(char *ptr, size_t size, size_t nmemb, void *userdata) -> size_t;
  };
}
