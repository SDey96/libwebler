#ifndef __WEBLER_DEPTH_HANDLER__

#define __WEBLER_DEPTH_HANDLER__

#include <vector>
#include <deque>
#include <string>
#include <regex>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <webler/Channel.hpp>
using namespace std;

#ifndef __WEBLER_CHANNEL_PREPROC__
#define __WEBLER_CHANNEL_PREPROC__
#define web_chan_ptr web::Channel<channel_data>*
#define web_chan web::Channel<channel_data>
#endif

namespace web {

	#ifndef __WEBLER_CHANNEL_DATA__
	
	#define __WEBLER_CHANNEL_DATA__
	// data type which will be passed through web::Channel
	struct channel_data {
		vector<string> links;
	};

	#endif

	/*
	* Class to crawl a specific depth
	**/
	class DepthHandler {

	public:
		/*
		* Constructor
		* @Params: (regex for the depth, channel to get, channel to put, true of its last depth)
		**/
		DepthHandler(string _regex_str, web_chan_ptr _chan_get, web_chan_ptr _chan_put, bool _is_end);
		
		/*
		* @Params: None
		* To start the process of crawling the depth
		**/
		void start();

	private:
		// channel to get data
		web_chan_ptr chan_get;

		// channel to send data
		web_chan_ptr chan_put;

		// regex string to match with html 
		string regex_str;

		// true if the depth is the last depth
		bool is_end;

	};

}

#endif