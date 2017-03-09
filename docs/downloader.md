# libwebler Downloader

### Include Downloader
``` #include <webler/Downloader.hpp> ```

Compile using ```--std=c++14 -lwebler -lpthread -lcurl```

Find out our tests [here](https://github.com/thecodesome/libwebler/blob/master/docs/tests.md)

### Methods included
* `class Downloader`
  * `auto download(string url, string outFile="") -> bool`
  * `auto SetProgressCallback(void (*callback)(double)) -> void;`

### Using of `Downloader`
> Disclaimer: All the links used below are just for demo and does not actually work. Check our tests for a working data.

#### Create WebCrawler object

```
webler::Downloader my_downloader;
```

#### Give the url and the name to save the file (optional)
* The url should be valid.
* THe default directory will be /home/usr/Downloads.

```
string url = "https://upload.wikimedia.org/wikipedia/commons/thumb/f/ff/Pizigani_1367_Chart_10MB.jpg/1200px-Pizigani_1367_Chart_10MB.jpg";

string filename = "image";
```

#### Open temporary files to write into
Give the temporary files random names to avoid conflict while downloading multiple files.
```
webler::Downloader::Downloader (){                                              
  downloaddir += "/Downloads";
  tempName = gen_random_string(6);
  for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {
    tempFileNames[i] = downloaddir + "/" + tempName + to_string(i+1);           
  }
}
```

#### Check the file extension and file size.
We are using libcurl library to determine these values. We will send a url request to the server and the required parameters to get the required values.
```
auto webler::Downloader::FileSize(const char *fileUrl) -> int {
  CURL *tempCurl = curl_easy_init();                                            
  CURLcode tempCode;
  if(CURLE_OK == tempCode) {
    tempCode = curl_easy_getinfo(tempCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD,&fullFileSize);
    if(CURLE_OK == tempCode && fullFileSize > 0.0){                           
      tempCode = curl_easy_getinfo(tempCurl, CURLINFO_CONTENT_TYPE, &contentType);
      extension = string(contentType);
      curl_easy_cleanup(tempCurl);
      return fullFileSize;
    }
  }
  return -1;
}
```

#### Setting callback for the data
To ensure that downlaod is complete, the callback will check for the download progress.
```
// Outside main

auto webler::Downloader::SetProgressCallback(void (*callback)(double)) -> void {
  progressCallback = callback;
}

// inside main

my_downloader.SetProgressCallback(progressCallback);

```

#### Start Downloading
```
auto webler::Downloader::DownloadFile (const char *url, const char *outFile) -> int {
  std::ofstream output(outFile, ios::binary);                                   
  fileSizeResult = FileSize(url);                                               
  FileRanges();                                                                 
  if (fileSizeResult > 0) {
      thread progressThread = thread(&Downloader::getDownloadProgress,this);    
      for (int i = 0; i < MAX_NO_OF_THREADS; i++) {
        fileThreadsToDownload[i] = thread(&Downloader::WriteTemporaryPartitions,this,i);
      }                                                                         
      for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {

        fileThreadsToDownload[i].join();                                                
        tempOutputFiles[i].close();                                             

      }
      progressThread.join();
      cout<<endl;
      MergeDownloadedPartitions();                                              
      return 0;
  }
  else{
      return -1;
  }
}
```

#### Download Progress
This will automatically calculates download at regular intervals.
```
auto webler::Downloader::getDownloadProgress()->void{
    if(progressCallback) {
      ifstream *files = new ifstream[MAX_NO_OF_THREADS];
      int indFileSize = 0;
      double percentComplete = 0;
      while(indFileSize!=fileSizeResult){
        indFileSize = 0;
        for (size_t i = 0; i < MAX_NO_OF_THREADS; i++) {                         
            ifstream file(tempFileNames[i].c_str(),ifstream::binary|ifstream::ate);
            indFileSize += file.tellg();
        }
        percentComplete = ((double)indFileSize/(double)fileSizeResult)*100;
        progressCallback(percentComplete);
        usleep(10000);
     }
  }
}
```

Check how to use Downloader in desktop app [here](https://github.com/thecodesome/libwebler/blob/master/docs/desktop_app.md)
