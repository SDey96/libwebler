# libwebler WebCrawler

### Include WebCrawler
``` #include <webler/WebCrawler.hpp> ```

Compile using ```--std=c++14 -lwebler -lpthread -lcurl```

Find out our tests [here](https://github.com/thecodesome/libwebler/blob/master/docs/tests.md)

### Methods included
* `class WebCrawler`
  * `int set_basedata(string _root_url, int _depth, vector<string> _regexes_str)`
  * `int set_concurrency_options(int _max_depth, int _depth_threads)`
  * `int set_callback(void (*_callback)(string, vector<string>))`
  * `bool is_in_progress()`
  * `int start()`
* `vector<string> get_href(string html)`

### Usage of `WebCrawler`
> Disclaimer: All the link and regex used below is just for demo and does not actually work. Check our tests for a working data.

#### Create WebCrawler object

```
web::WebCrawler my_crawler;
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

if(response_code == web::WC_SUCCESS) {
    cout << "It was a success!" << endl;
} else if (response_code == web::WC_EINPROGRESS) {
    cout << "My crawler has started crawling, so I cant alter base data." << endl;
} else if (response_code == web::WC_EINVALID) {
    cout << "Some problem in depth or number of regex." << endl;
}
```

#### Set Concurrency Options
To set number of depths crawling concurrently and number of threads crawling in each depth. Default is set to `3 depth` and `5 threads` in each depth
> Note: Set this according to server and your system capacity. Overloading the server may result in you getting blocked.

```
response_code = my_crawler.set_concurrency_options(3,2);

if(response_code == web::WC_SUCCESS) {
    cout << "It was a success!" << endl;
} else if (response_code == web::WC_EINPROGRESS) {
    cout << "My crawler has started crawling." << endl;
} else if (response_code == web::WC_EINVALID) {
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
if(response_code == web::WC_SUCCESS) {
    cout << "It was a success!" << endl;
} else if (response_code == web::WC_EINPROGRESS) {
    cout << "My crawler has started crawling." << endl;
}
```
#### Start crawling
```
response_code = my_crawler.start();

if(response_code == web::WC_SUCCESS) {
    cout << "Crawler finished successfully!" << endl;
} else if (response_code == web::WC_EINPROGRESS) {
    cout << "My crawler is already in progress." << endl;
} else if (response_code == web::WC_EINVALID) {
    cout << "Basedata is not set." << endl;
} else if (response_code == web::WC_EUNKNOWN) {
    cout << "Unknown error occured." << endl;
}
```

#### Collecting failed URL
Use this after crawling is finished
```
vector<web::failed_url> all_failed_url = my_crawler.get_failed_url();
for(auto i: all_failed_url) {
    cout << "Depth at which it failed:" << i.depth << endl;
    cout << "Failed URL:" << i.url << endl;
}
```
### Usage of `get_href`
This function is to scrape out all the links from `<a href="link"></a>` tags of html.
```
string html = "here will be my html";

// vector containing all the links
vector<string> hrefs = get_href(html);
```
