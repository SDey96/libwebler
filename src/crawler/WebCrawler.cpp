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
	metadata_updated(false)
	{}

bool web::WebCrawler::set_metadata(string rt_url, int d, vector<string> regexes) {

	if(!is_in_progress() || regexes.size() != d){
		return false;
	}

	root_url = rt_url;
	depth = d;
	regexes_str = regexes;
	metadata_updated = true;
	return true;

}


bool web::WebCrawler::set_concurrency_options(int d, int t) {

	if(!is_in_progress()){
		return false;
	}

	max_depth = d;
	depth_threads = t;

	return true;

}

bool web::WebCrawler::set_callback(void (*cb)(bool, string, vector<string>)) {
	if(!is_in_progress()) {
		return false;
	}

	callback = cb;
	return true;
}

bool web::WebCrawler::is_in_progress() {
	return in_progress;
}

bool web::WebCrawler::start() {

	if(!metadata_updated || !is_in_progress()){
		return false;
	}

	in_progress = true;

	web_chan_ptr* channels = new web_chan_ptr[depth+1];

	web_chan_ptr end_channel = channels[depth];

	web::DepthPoolManager main_dpm(depth,depth_threads);

	web::channel_data root_data;
	root_data.links.push_back(root_url);
	channels[0]->add(root_data);

	for(int i=1; i<depth; i++) {
		main_dpm.add_depth(regexes_str[i-1],channels[i-1],channels[i],false);
	}

	main_dpm.add_depth(regexes_str[depth-1],channels[depth-1],end_channel,true);

	bool closed;
	string url;
	for(web::channel_data data=end_channel->retrieve(&closed); !closed ; data=end_channel->retrieve(&closed)) {
		url = data.links.back();
		data.links.pop_back();
		callback(true,url,data.links);
	}

	return true;

}
