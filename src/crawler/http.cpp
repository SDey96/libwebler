#include <curl/curl.h>
#include <crawler/http.hpp>
using namespace std;

size_t curl_writeback(void *contents, size_t size, size_t nmemb, string *s) {
    size_t newLength = size*nmemb;
    size_t oldLength = s->size();
    try {
        // resizing string
        s->resize(oldLength + newLength);
    } catch(bad_alloc &e) {
        return 0;
    }
    // appending new received data
    copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
    return size*nmemb;
}

webler::http_response webler::http_get(string url) {
    http_response response;
    try {
        CURL *curl;
        CURLcode res;
     
        curl = curl_easy_init();

        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writeback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.html); 
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
     
            res = curl_easy_perform(curl);

            if(res != CURLE_OK){
                response.success = false;
            }
     
            curl_easy_cleanup(curl);
        } else {
            response.success = false;
        }
    } catch (const char* err) {
        response.success = false;
    }
    
    return response;

}