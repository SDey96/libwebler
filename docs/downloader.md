# libwebler Downloader

### Include Downloader
``` #include <webler/Downloader.hpp> ```

Compile using ```--std=c++14 -lwebler -lpthread -lcurl```

Find out our tests [here](https://github.com/thecodesome/libwebler/blob/master/docs/tests.md)

### Publc methods included
* `class Downloader`
  * `auto download(string url, string outFile="") -> bool`
  * `auto SetProgressCallback(void (*callback)(double)) -> void;`

### Using `Downloader`

#### Create Downloader object

```
webler::Downloader my_downloader;
```

#### Give the url and the name to save the file (optional)
* The url should be valid.
* THe default directory will be $HOME/Downloads.

```
string url = "https://upload.wikimedia.org/wikipedia/commons/f/ff/Pizigani_1367_Chart_10MB.jpg";

string filename = "image";
```

#### Setting callback for checking progress
The callback function will be called with the download percentage at regular intervals of time.
```
// Outside main

void progressCallback(double percentage) {
    printf("Download Progress : %.3f\r",percentage);
}

// inside main

my_downloader.SetProgressCallback(progressCallback);

```

#### Start Downloading
```
my_downloader.download(url,filename);
/*
filename is optional
my_downloader.download(url) also works but gives a random name.
*/
```

Check how to use Downloader in desktop app [here](https://github.com/thecodesome/libwebler/blob/master/docs/desktop_app.md#downloader)
