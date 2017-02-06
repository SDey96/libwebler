#include <string>
#include <curl/curl.h>
#include "http.hpp"
using namespace std;

size_t curl_writeback(void *contents, size_t size, size_t nmemb, string *s) {
	size_t newLength = size*nmemb;
	size_t oldLength = s->size();
	try {
		s->resize(oldLength + newLength);
	} catch(bad_alloc &e) {
		return 0;
	}
	copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
	return size*nmemb;
}

web::http_response web::http_get(string url) {
	CURL *curl;
	CURLcode res;
 
	curl = curl_easy_init();
	http_response response;

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

	return response;

}