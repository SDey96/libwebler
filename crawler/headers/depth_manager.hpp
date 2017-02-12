#ifndef __WEB_DEPTH_MANAGER__

#define __WEB_DEPTH_MANAGER__

#include <vector>
#include <deque>
#include <string>
#include <regex>
#include <thread>
#include <semaphore.h>
#include <web/channel.hpp>
using namespace std;

namespace web {

	struct channel_data {
		vector<string> links; // URL
	};

	struct running_pool_data {
		thread* threads; // array of threads
		int pool_index; // index in the pool
		int threads_active; // number of threads active
	};

	struct wait_pool_data {
		Channel<channel_data> *chan_get; // channel to get data
		Channel<channel_data> *chan_put; // channel to send data
		string regex_str; // string of regex
	};

	class DepthHandler {

	private:
		Channel<channel_data> *chan_get; // channel to get data
		Channel<channel_data> *chan_put; // channel to send data
		string regex_str; // string of regex
		void (*end_callback)();

	public:
		DepthHandler(string,Channel<channel_data>*,Channel<channel_data>*,void (*)());
		void start(); // start making HTTP requests

	};

	// function to run on DepthPoolManager thread
	void DepthPoolThreadFunction(string, Channel<channel_data>*, Channel<channel_data>*, void (*)());

	class DepthPoolManager {

	private:
		int max_pool_size; // max number of depth to crawl at a time
		int depth_thread_count; // number of threads to create for each depth
		sem_t pool_size_handler; // to maintain pool size

		deque<running_pool_data*> running_pool;
		deque<wait_pool_data*> wait_pool;
		bool in_progress;

	public:
		DepthPoolManager(int,int); // sets max pool size and thread count
		void add_new_depth(string,Channel<channel_data>*,Channel<channel_data>*);

	};

}

#endif