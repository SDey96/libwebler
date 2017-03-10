# libwebler WebCrawler

### Include WebCrawler
``` #include <webler/WebCrawler.hpp> ```

Compile using ```--std=c++14 -lwebler -lpthread -lcurl```

Find out our tests [here](https://github.com/thecodesome/libwebler/blob/master/docs/tests.md)

### Public methods included
* `class WebCrawler`

  * `int set_basedata(string _root_url, int _depth, vector<string> _regexes_str)`
    * Used to set the base URL, depth and all the regex. This function has to be called successfully to run web crawler.
    * Returns exit status
      * `webler::WC_EINPROGRESS` if crawler is in progress.
      * `webler::WC_SUCCESS` for success.
      * `webler::WC_EINVALID` for invalid depth or invalid number of regex.

  * `int set_concurrency_options(int _max_depth, int _depth_threads)`
    * Used to set the number of depths running a time and number of threads to be created in each depth.
    * Returns exit status
      * `webler::WC_EINPROGRESS` if crawler is in progress.
      * `webler::WC_SUCCESS` for success.
      * `webler::WC_EINVALID` for <=0 values.

  * `int set_callback(void (*_callback)(string, vector<string>))`
    * Used to set the callback for the data received from the final depth.
    * Returns exit status
      * `webler::WC_EINPROGRESS` if crawler is in progress.
      * `webler::WC_SUCCESS` for success.

  * `bool is_in_progress()`
    * Returns `true` if the crawler is in progress. Else `false`

  * `int start()`
    * Used to start the crawler.
    * Returns exit status
      * `webler::WC_EINPROGRESS` if crawler is in progress.
      * `webler::WC_EINVALID` if basedata is not updated.
      * `webler::WC_SUCCESS` for success.
      * `webler::WC_EUNKNOWN` for unknown error.


* `vector<string> get_href(string html)` // non class member

### Using `WebCrawler`
> Disclaimer: All the link and regex used below is just for demo and does not actually work. Check our tests for a working data.

#### Create WebCrawler object

```
webler::WebCrawler my_crawler;
```

#### Gather your depth and regular expression strings
* Regex vector size should be equal to depth
* vector should contain the regexes in the order of the depth.
  * Ex: The first regex should be for the starting URL to get a part of HTML after a GET request. Second regex should be for HTML got from links scraped from starting URL. Last regex should be for getting the final data from last depth.
* Each regex should capture some part of HTML at that depth in order to go to next depth
* Last regex is for getting the final data from last depth

```
int crawl_depth = 3;

vector<string> my_regex = {
    string("regex[^for]firstURL(IWillCaptureThis)fromRootURL"),
    string("regex[^for]secondURL(IWillCaptureThis)and(this)"),
    string("regex[^for]myFinal(data)and(data2)"),
};
```

#### Set the base data
Starting URL, depth and regular expressions
```
int response_code = my_crawler.set_basedata(
    string("http://www.myurl.com/starting/point/"), // starting URL
    crawl_depth, // depth
    my_regex // 3 regex
);

if(response_code == webler::WC_SUCCESS) {
    cout << "It was a success!" << endl;
} else if (response_code == webler::WC_EINPROGRESS) {
    cout << "My crawler has started crawling, so I cant alter base data." << endl;
} else if (response_code == webler::WC_EINVALID) {
    cout << "Some problem in depth or number of regex." << endl;
}
```

#### Set Concurrency Options
To set number of depths crawling concurrently and number of threads crawling in each depth. Default is set to `3 depth` and `5 threads` in each depth
> Note: Set this according to server and your system capacity. Overloading the server may result in you getting blocked.

```
response_code = my_crawler.set_concurrency_options(3,2);

if(response_code == webler::WC_SUCCESS) {
    cout << "It was a success!" << endl;
} else if (response_code == webler::WC_EINPROGRESS) {
    cout << "My crawler has started crawling." << endl;
} else if (response_code == webler::WC_EINVALID) {
    cout << "Numbers are not >0" << endl;
}
```

#### Setting callback for the data
To handle the data obtained from last depth. Callback will be run everytime a URL from the last depth is crawled.

```
// Outside main
void callback(string url, vector<string> data) {
    cout << "This last depth was a success" << endl;

    cout << "URL from last depth: " << url << endl;

    // 'data' has all the captures of last depth for that particular URL in a vector
    cout << "Data from the this URL:" << endl;
    for(auto i: data) {
      cout << i << endl;
    }
}

// inside main
response_code = my_crawler.set_callback(callback);
if(response_code == webler::WC_SUCCESS) {
    cout << "It was a success!" << endl;
} else if (response_code == webler::WC_EINPROGRESS) {
    cout << "My crawler has started crawling." << endl;
}
```
#### Start crawling
```
response_code = my_crawler.start();

if(response_code == webler::WC_SUCCESS) {
    cout << "Crawler finished successfully!" << endl;
} else if (response_code == webler::WC_EINPROGRESS) {
    cout << "My crawler is already in progress." << endl;
} else if (response_code == webler::WC_EINVALID) {
    cout << "Basedata is not set." << endl;
} else if (response_code == webler::WC_EUNKNOWN) {
    cout << "Unknown error occured." << endl;
}
```

#### Collecting failed URL
Use this after crawling is finished
```
vector<webler::failed_url> all_failed_url = my_crawler.get_failed_url();
for(auto i: all_failed_url) {
    cout << "Depth at which it failed:" << i.depth << endl;
    cout << "Failed URL:" << i.url << endl;
}
```
### Using of `get_href`
This function is to scrape out all the links from `<a href="link"></a>` tags of html.
```
string html = "here will be my html";

// vector containing all the links
vector<string> hrefs = get_href(html);
```

Check how to use WebCrawler in desktop app [here](https://github.com/thecodesome/libwebler/blob/master/docs/desktop_app.md#web-crawler)
