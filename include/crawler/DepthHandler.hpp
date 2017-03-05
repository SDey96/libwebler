#ifndef __WEBLER_DEPTH_HANDLER_HPP__

#define __WEBLER_DEPTH_HANDLER_HPP__

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
#define web_chan_ptr webler::Channel<channel_data>*
#define web_chan webler::Channel<channel_data>
#endif /*__WEBLER_CHANNEL_PREPROC__*/

namespace webler {

	#ifndef __WEBLER_CHANNEL_DATA__
	#define __WEBLER_CHANNEL_DATA__
	// data type which will be passed through webler::Channel
	struct channel_data {
		vector<string> links;
	};
	#endif /*__WEBLER_CHANNEL_DATA__*/

	#ifndef __WEBLER_FAILED_URL__
	#define __WEBLER_FAILED_URL__
	struct failed_url {
		string url;
		int depth;
		failed_url() {}
		failed_url(string u, int d): url(u), depth(d) {}
	};
	#endif /*__WEBLER_FAILED_URL__*/

	/*
	* Class to crawl a specific depth
	**/
	class DepthHandler {

	public:
		/*
		* Constructor
		* @Params: (depth,regex for the depth, channel to get, channel to put, true of its last depth, channel to collect error url)
		**/
		DepthHandler(int d, string _regex_str, web_chan_ptr _chan_get, web_chan_ptr _chan_put, bool _is_end,Channel<failed_url>* _failed_urls);
		
		/*
		* @Params: None
		* To start the process of crawling the depth
		**/
		void start();

	private:
		// depth
		int depth;

		// channel to get data
		web_chan_ptr chan_get;

		// channel to send data
		web_chan_ptr chan_put;

		// regex string to match with html 
		string regex_str;

		// true if the depth is the last depth
		bool is_end;

		// channel to collect failed URL
		Channel<failed_url>* chan_failed_urls;

	}; /*class DepthHandler*/


} /*namespace webler*/

#endif /*__WEBLER_DEPTH_HANDLER_HPP__*/