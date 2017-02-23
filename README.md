# Web Crawler and Downloader

## Install
```
$ git clone https://github.com/thecodesome/web.git
$ cd web/
$ sudo make install
$ make clean
```

## How to use?
### Include
```
#include <web/http.hpp>
#include <web/Channel.hpp>
#include <web/WebCrawler.hpp>
```

### Compile
`--std=c++14 /usr/local/lib/web/web.a -lpthread -lcurl`
```
$ g++ -o executable program.cpp --std=c++14 /usr/local/lib/web/web.a -lpthread -lcurl
```

## Tests
> You will need python and Flask to use our testing

From the cloned repo and inside `web` folder, follow these steps

#### Start the server
```
$ cd test/test_server/
$ python server.py
```

#### Start the test
In another terminal tab, with directory set to the `web` folder, run the below command
```
$ make test
```
You will receive `1024` callbacks from WebCrawler

> Downloader and complete docs coming soon ...
