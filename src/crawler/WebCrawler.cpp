#include <regex>
#include <crawler/WebCrawler.hpp>
#include <crawler/DepthManager.hpp>
using namespace std;

webler::WebCrawler::WebCrawler(): 
    callback(NULL), 
    depth(0), 
    root_url(""), 
    in_progress(false),
    max_depth(3),
    depth_threads(5),
    basedata_updated(false) {}

int webler::WebCrawler::set_basedata(string _root_url, int _depth, vector<string> _regexes_str) {

    if(is_in_progress()){
        return webler::WC_EINPROGRESS;
    }

    if(_depth<=0 || _regexes_str.size() != _depth){
        return webler::WC_EINVALID;
    }

    root_url = _root_url;
    depth = _depth;
    regexes_str = _regexes_str;
    basedata_updated = true;
    return webler::WC_SUCCESS;

}


int webler::WebCrawler::set_concurrency_options(int _max_depth, int _depth_threads) {

    if(is_in_progress()){
        return webler::WC_EINPROGRESS;
    }

    if(_max_depth<=0 || _depth_threads<=0){
        return webler::WC_EINVALID;
    }

    max_depth = _max_depth;
    depth_threads = _depth_threads;

    return webler::WC_SUCCESS;

}

int webler::WebCrawler::set_callback(void (*_callback)(string, vector<string>)) {
    if(is_in_progress()){
        return webler::WC_EINPROGRESS;
    }

    callback = _callback;
    return webler::WC_SUCCESS;
}

bool webler::WebCrawler::is_in_progress() {
    return in_progress;
}

int webler::WebCrawler::start() {

    try {

        if(is_in_progress()){
            return webler::WC_EINPROGRESS;
        }

        if(!basedata_updated){
            return webler::WC_EINVALID;
        }

        // setting flag to in progress
        in_progress = true;

        // creating channels to communicate between depths
        vector<web_chan_ptr> channels;
        channels.reserve(depth);
        for(int i=0; i<=depth; i++) {
            channels.push_back(new web_chan);
        }

        // channel to get data from last depth
        web_chan_ptr end_channel = channels[depth];

        // channel to collect the failed URLs
        Channel<failed_url> chan_failed_url;

        // The main DepthPoolManager object for web crawler
        webler::DepthPoolManager main_dpm(max_depth,depth_threads,&chan_failed_url);

        // adding the first URL in the channel
        webler::channel_data root_data;
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
            for(webler::channel_data data=end_channel->retrieve(&closed); 
                !closed ; 
                data=end_channel->retrieve(&closed)) {

                url = data.links.back();
                data.links.pop_back();
                callback(url,data.links);

            }
        } else {
            for(webler::channel_data data=end_channel->retrieve(&closed); 
                !closed ; 
                data=end_channel->retrieve(&closed)) {}
        }
        

        // deleting all the channels
        for(int i=0; i<=depth; i++) {
            delete channels[i];
        }
        channels.clear();

        // collecting all failed URL
        chan_failed_url.close();
        for(webler::failed_url data=chan_failed_url.retrieve(&closed); 
            !closed ; 
            data=chan_failed_url.retrieve(&closed)) {

            failed_urls.push_back(data);

        }

        in_progress = false;

        return webler::WC_SUCCESS;

    } catch (...) {

        return webler::WC_EUNKNOWN;
        
    }


}

vector<webler::failed_url> webler::WebCrawler::get_failed_url() {
    return failed_urls;
}

vector<string> webler::get_href(string html) {
    regex link_extractor("<\\s*a\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", regex::icase);
    vector<string> extracted_urls;

    copy(
        sregex_token_iterator( html.begin(), html.end(), link_extractor, 1 ),
        sregex_token_iterator(),
        back_inserter(extracted_urls)
    );

    return extracted_urls;

}