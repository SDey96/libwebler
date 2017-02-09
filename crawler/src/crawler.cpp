#include <iostream>
#include <vector>
#include <string>
#include <web/crawler.hpp>
using namespace std;

web::WebCrawler::WebCrawler() {
	callback = NULL;
	depth = 0;
	root_url = "";
}

bool web::WebCrawler::set_options(string rt_url, int d, vector<string> regexes) {

	if(regexes.size() != d) {
		return false;
	}

	root_url = rt_url;
	depth = d;
	regexes_str = regexes;

	return true;

}

void web::WebCrawler::set_callback(void (*cb)(bool, string, string)) {
	callback = cb;
}

