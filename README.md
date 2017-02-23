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

From the cloned repo and inside `libwebler` folder, follow these steps

#### Start the server
```
$ cd test/test_server/
$ python server.py
```

#### Start the test
In another terminal tab, with directory set to the `libwebler` folder, run the below command
```
$ make test
```
You will receive `1024` callbacks from WebCrawler

> Downloader and complete docs coming soon ...
