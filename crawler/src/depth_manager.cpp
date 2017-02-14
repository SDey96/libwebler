#include <web/depth_manager.hpp>
#include <web/http.hpp>
#include <iostream>
using namespace std;

void web::DepthPoolThreadFunction(string regex_str,web::Channel<channel_data>* chanGet,web::Channel<channel_data>* chan_put, void (*callback)()){

}

/*######### DepthHandler #########*/
web::DepthHandler::DepthHandler(string rx_str, web::Channel<channel_data>* chanGet, web::Channel<channel_data>* chanPut, void (*callback)()){

	regex_str = rx_str;
	chan_get = chanGet;
	chan_put = chanPut;
	end_callback = callback;

}

void web::DepthHandler::start() {

	vector<string> channel_urls,extracted_urls;

	// to match links
	regex link_extractor("<\\s*a\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", regex::icase);
	
	// to match html
	regex html_extractor(regex_str.c_str(), regex::icase);
	smatch html_match;

	// response from GET request
	web::http_response res;

	// HTML after filtering with regex
	string filtered_html;

	int match_length;

	web::channel_data send_data;

	bool closed;

	// getting urls from channel
	for(web::channel_data it; !closed ; it=chan_get->retrieve(&closed)) {
		// link got from channel
		channel_urls = it.links;

		// iterating over the links
		for(vector<string>::iterator url=channel_urls.begin(); url!=channel_urls.end(); ++url ) {
			
			// GET requests to the link
			res = web::http_get(*url);

			if(res.success) { // GET request was successful

				// filtering the HTML
				if( regex_search(res.html, html_match, html_extractor) && html_match.size()>1 ){
					
					match_length = html_match.size();
					filtered_html = "";

					for(int i=1; i<=match_length; i++) {
						filtered_html += html_match.str(i);
					}

					// extracting URL
					extracted_urls.clear();
					copy(
					    sregex_token_iterator( filtered_html.begin(), filtered_html.end(), link_extractor, 1 ),
					    sregex_token_iterator(),
					    back_inserter(extracted_urls)
				    );

				    send_data.links = extracted_urls;
				    chan_put->add(send_data);
					
				}

			} else {
				// TODO handle faliure
				cout << "Fail: " << *url << endl;
			}

		}

	}
	if(end_callback){
		end_callback();
	}

}

/*######### DepthPoolManager #########*/
web::DepthPoolManager::DepthPoolManager(int pool_size, int thread_count) {

	max_pool_size = pool_size;
	depth_thread_count = thread_count;	

}

void web::DepthPoolManager::add_new_depth(string regex_str, web::Channel<channel_data>* chanGet, web::Channel<channel_data>* chanPut) {

}