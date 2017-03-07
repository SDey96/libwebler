#ifndef __WEBLER_HTTP_HPP__

#define __WEBLER_HTTP_HPP__

#include <string>
using namespace std;

namespace webler {

    struct http_response {
        string html; // response HTML
        bool success; // true if GET request was successful
        http_response(): success(true), html("") {}
    };

    /*
    * @Params: (URL)
    * returns http_response datatype
    **/
    http_response http_get(string url);
}

#endif