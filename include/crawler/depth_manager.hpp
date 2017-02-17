#ifndef __WEB_DEPTH_MANAGER__

#define __WEB_DEPTH_MANAGER__

#include <vector>
#include <deque>
#include <string>
#include <regex>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <web/channel.hpp>
using namespace std;

#define web_chan web::Channel<channel_data>*

namespace web {

	struct channel_data {
		vector<string> links; // URL
	};

	struct running_pool_data {
		int _id;
		thread* threads; // array of threads
		web_chan chan_get; // channel to get data
		int threads_active; // number of threads active
		running_pool_data() :threads_active(0) {}
	};

	struct wait_pool_data {
		string regex_str; // string of regex
		web_chan chan_get; // channel to get data
		web_chan chan_put; // channel to send data

		wait_pool_data() {}
		wait_pool_data(
			string rx_str,
			web_chan chanGet,
			web_chan chanPut) {

			chan_get = chanGet;
			chan_put = chanPut;
			regex_str = rx_str;
		}
	};

	class DepthHandler {

	private:
		web_chan chan_get; // channel to get data
		web_chan chan_put; // channel to send data
		string regex_str; // string of regex
	

	public:
		DepthHandler(string,web_chan,web_chan);
		void start(); // start making HTTP requests

	};

	// function to run on DepthPoolManager thread
	void __DepthPoolThreadFunction(string, web_chan, web_chan, function<void(web_chan,int)>,int);

	class DepthPoolManager {

	private:
		int max_pool_size; // max number of depth to crawl at a time
		int thread_count; // number of threads to create for each depth
		sem_t pool_size_handler; // to maintain pool size
		mutex pool_mutex;
		int rp_count;

		deque<running_pool_data> running_pool;
		deque<wait_pool_data> wait_pool;
		bool in_progress;

		running_pool_data new_depth_util(string,web_chan,web_chan);

	public:
		DepthPoolManager(int,int); // sets max pool size and thread count
		void add_new_depth(string,web_chan,web_chan);

	};

}

#endif