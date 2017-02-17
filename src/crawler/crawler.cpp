#include <crawler/crawler.hpp>
using namespace std;

web::WebCrawler::WebCrawler(): 
	callback(NULL), 
	depth(0), 
	root_url(""), 
	in_progress(false) 
	{}

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

bool web::WebCrawler::is_in_progress() {
	return in_progress;
}

bool web::WebCrawler::start() {
	return true;
}

bool web::WebCrawler::stop() {
	return true;
}

