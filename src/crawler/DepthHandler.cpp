#include <iostream>
#include <crawler/http.hpp>
#include <crawler/DepthHandler.hpp>
using namespace std;

/*######### DepthHandler #########*/
web::DepthHandler::DepthHandler(string _regex_str, web_chan_ptr _chan_get, web_chan_ptr _chan_put, bool _is_end){

	regex_str = _regex_str;
	chan_get = _chan_get;
	chan_put = _chan_put;
	is_end = _is_end;

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
	for(web::channel_data it=chan_get->retrieve(&closed); !closed ; it=chan_get->retrieve(&closed)) {
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
					extracted_urls.clear();
					if(is_end) {
						
						for(int i=1; i<=match_length; i++) {
							extracted_urls.push_back(html_match.str(i));
						}
						extracted_urls.push_back(*url);

					} else {
						filtered_html = "";

						for(int i=1; i<=match_length; i++) {
							filtered_html += html_match.str(i);
						}

						// extracting URL
						copy(
						    sregex_token_iterator( filtered_html.begin(), filtered_html.end(), link_extractor, 1 ),
						    sregex_token_iterator(),
						    back_inserter(extracted_urls)
					    );

					}
				    send_data.links = extracted_urls;
					chan_put->add(send_data);
					
				}

			} else {
				// TODO handle faliure
				// cout << "Fail: " << *url << endl;
			}

		}

	}
}