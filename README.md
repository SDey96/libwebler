# Web Crawler and Downloader

## Install
```
$ git clone https://github.com/thecodesome/libwebler.git
$ cd libwebler/
$ sudo make install
$ make clean
```

## How to use?
### Include
```
#include <webler/WebCrawler.hpp>
```

### Compile
`--std=c++14 -lwebler -lpthread -lcurl`
```
$ g++ -o executable program.cpp --std=c++14 -lwebler -lpthread -lcurl
```

## Tests
> You will need python and Flask to use our testing

#### Start the server
```
$ make start_server
```

#### Start the test
In another terminal, run the below command
```
$ make test
```
You will receive `1024` callbacks from WebCrawler

> Downloader and complete docs coming soon ...
