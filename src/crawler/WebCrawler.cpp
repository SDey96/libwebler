#include <crawler/WebCrawler.hpp>
#include <crawler/DepthManager.hpp>
using namespace std;

web::WebCrawler::WebCrawler(): 
	callback(NULL), 
	depth(0), 
	root_url(""), 
	in_progress(false),
	max_depth(3),
	depth_threads(2),
	basedata_updated(false) {}

bool web::WebCrawler::set_basedata(string _root_url, int _depth, vector<string> _regexes_str) {

	if(is_in_progress() || _depth<=0 || _regexes_str.size() != _depth){
		return false;
	}

	root_url = _root_url;
	depth = _depth;
	regexes_str = _regexes_str;
	basedata_updated = true;
	return true;

}


bool web::WebCrawler::set_concurrency_options(int _max_depth, int _depth_threads) {

	if(is_in_progress() || _max_depth<=0 || _depth_threads<=0){
		return false;
	}

	max_depth = _max_depth;
	depth_threads = _depth_threads;

	return true;

}

bool web::WebCrawler::set_callback(void (*_callback)(bool, string, vector<string>)) {
	if(is_in_progress()) {
		return false;
	}

	callback = _callback;
	return true;
}

bool web::WebCrawler::is_in_progress() {
	return in_progress;
}

bool web::WebCrawler::start() {

	if(!basedata_updated || is_in_progress()){
		return false;
	}

	// setting flag to in progress
	in_progress = true;

	// array of channels to be used in DepthPoolManager
	web_chan_ptr* channels = new web_chan_ptr[depth+1];
	for(int i=0; i<=depth; i++) {
		channels[i] = new web_chan;
	}

	// channel to get data from last depth
	web_chan_ptr end_channel = channels[depth];

	// The main DepthPoolManager object for web crawler
	web::DepthPoolManager main_dpm(max_depth,depth_threads);

	// adding the first URL in the channel
	web::channel_data root_data;
	root_data.links.push_back(root_url);
	channels[0]->add(root_data);
	
	// Adding the depths
	for(int i=1; i<depth; i++) {
		// depths other than the last depth
		main_dpm.add_depth(regexes_str[i-1],channels[i-1],channels[i],false);
	}
	// adding last depth
	main_dpm.add_depth(regexes_str[depth-1],channels[depth-1],end_channel,true);

	// closing the first sending channel
	channels[0]->close();

	// getting the data from the last channel
	bool closed;
	string url;
	if(callback){ // If callback is set
		for(web::channel_data data=end_channel->retrieve(&closed); 
			!closed ; 
			data=end_channel->retrieve(&closed)) {

			url = data.links.back();
			data.links.pop_back();
			callback(true,url,data.links);

		}
	} else {
		for(web::channel_data data=end_channel->retrieve(&closed); 
			!closed ; 
			data=end_channel->retrieve(&closed)) {}
	}

	for(int i=0; i<=depth; i++) {
		delete channels[i];
	}
	delete channels;

	while(!main_dpm.cleanup());

	return true;

}
