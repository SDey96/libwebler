#ifndef __WEB_DEPTH_MANAGER__

#define __WEB_DEPTH_MANAGER__

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

	struct running_pool_thread_data {
		thread _t;
		bool is_active;
		running_pool_thread_data(): is_active(false) {}
	};

	struct running_pool_data {
		int _id;
		running_pool_thread_data* threads; // array of threads
		web_chan_ptr chan_get; // channel to get data
		int threads_active; // number of threads active
		running_pool_data() :threads_active(0) {}
	};

	struct wait_pool_data {
		string regex_str; // string of regex
		web_chan_ptr chan_get; // channel to get data
		web_chan_ptr chan_put; // channel to send data
		bool is_end;

		wait_pool_data() {}
		wait_pool_data(
			string rx_str,
			web_chan_ptr chanGet,
			web_chan_ptr chanPut,
			bool isend) {

			chan_get = chanGet;
			chan_put = chanPut;
			regex_str = rx_str;
			is_end = isend;
		}
	};


	// function to run on DepthPoolManager thread
	void __DepthPoolThreadFunction(string, web_chan_ptr, web_chan_ptr, function<void(web_chan_ptr,int)>,int,bool);

	class DepthPoolManager {

	private:
		int max_pool_size; // max number of depth to crawl at a time
		int thread_count; // number of threads to create for each depth
		sem_t pool_size_handler; // to maintain pool size
		mutex pool_mutex;
		int rp_count;

		deque<running_pool_data> running_pool;
		deque<wait_pool_data> wait_pool;
		bool end_added;

		function<void(web_chan_ptr,int)> *end_callback;

	public:
		DepthPoolManager(int,int); // sets max pool size and thread count
		void add_depth(string,web_chan_ptr,web_chan_ptr,bool);

	};

}

#endif