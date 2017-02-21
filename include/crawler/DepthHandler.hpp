#ifndef __WEB_DEPTH_HANDLER__

#define __WEB_DEPTH_HANDLER__

#include <vector>
#include <deque>
#include <string>
#include <regex>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <web/Channel.hpp>
using namespace std;

#ifndef __WEB_CHANNEL_PREPROC__
#define __WEB_CHANNEL_PREPROC__
#define web_chan_ptr web::Channel<channel_data>*
#define web_chan web::Channel<channel_data>
#endif

namespace web {

	#ifndef __WEB_CHANNEL_DATA__
	#define __WEB_CHANNEL_DATA__
	struct channel_data {
		vector<string> links; // URL
	};
	#endif

	class DepthHandler {

	private:
		web_chan_ptr chan_get; // channel to get data
		web_chan_ptr chan_put; // channel to send data
		string regex_str; // string of regex
		bool is_end;
	

	public:
		DepthHandler(string,web_chan_ptr,web_chan_ptr,bool);
		void start(); // start making HTTP requests

	};

}

#endif