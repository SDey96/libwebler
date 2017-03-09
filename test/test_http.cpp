#include <iostream>
#include <string>
#include <vector>
#include <webler/http.hpp>
#include <webler/WebCrawler.hpp>
using namespace std;

int main () {

    webler::http_response r;
    r = webler::http_get("http://example.com/");
    if(r.success) {
        cout << r.html << endl;
    } else {
        cout << "Error occured" << endl;        
    }

    cout << "HTTP GET test is over" << endl;

    return 0;
}