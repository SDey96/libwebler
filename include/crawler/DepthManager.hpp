#ifndef __WEBLER_DEPTH_MANAGER_HPP__

#define __WEBLER_DEPTH_MANAGER_HPP__

#include <vector>
#include <deque>
#include <string>
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

	// single thread data stroed in running_pool_data
	struct running_pool_thread_data {
		thread _t; // the thread
		bool is_active; // true if the thread is active
		running_pool_thread_data(): is_active(false) {}
	};

	// Data type stored in the running pool of DepthPoolManager
	struct running_pool_data {
		int _id; // ID for the pool data
		running_pool_thread_data* threads; // array of threads for that depth
		web_chan_ptr chan_put; // channel to put data
		int threads_active; // number of threads active in that depth
		running_pool_data() :threads_active(0) {}
	};

	// Data type stored in the waiting pool of DepthPoolManager
	struct wait_pool_data {
		string regex_str; // regex string for the depth
		web_chan_ptr chan_get; // channel to get data
		web_chan_ptr chan_put; // channel to send data
		bool is_end; // true of it is the last depth

		wait_pool_data() {}
		wait_pool_data(
			string _regex_str,
			web_chan_ptr _chan_get,
			web_chan_ptr _chan_put,
			bool _is_end) {

			chan_get = _chan_get;
			chan_put = _chan_put;
			regex_str = _regex_str;
			is_end = _is_end;
		}
	};


	// function to run on DepthPoolManager thread
	void __DepthPoolThreadFunction(
		string regex_str, // regex for the depth
		web_chan_ptr chan_get, // channel to get data
		web_chan_ptr chan_put,  // channel to send data
		function<void(int)> callback, // function to call at the end of thread
		int rpd_id, // running pool id
		bool is_end, // true if it is the end depth
		Channel<failed_url>* chan_failed_url // channel for failed URL
	);

	/*
	* Class to manage DepthHandler objects in a pool of threads to crawl
	**/
	class DepthPoolManager {

	public:
		/*
		* Constructor
		* @Params: (max_pool_size, thread_count,channel to collet failed URL)
		**/
		DepthPoolManager(int _max_pool_size,int _thread_count,Channel<failed_url>* _failed_urls);

		~DepthPoolManager();

		/*
		* @Params: (regex for new depth, channel to get, channel to put, true of its last depth)
		* returns false if the end depth has beed added already, else true
		**/
		bool add_depth(string regex_str, web_chan_ptr _chan_get, web_chan_ptr _chan_put, bool is_end);

	private:
		// max number of depth to crawl at a time
		int max_pool_size;

		// number of threads to create for each depth 
		int thread_count;

		// to maintain data concurrency of pool queues
		mutex pool_mutex;

		// total depth added to running pool
		int rp_count;

		// holds the data of currently running depths
		deque<running_pool_data> running_pool;

		// holds the data of depths to crawl later
		deque<wait_pool_data> wait_pool;

		// true if the end depth is added
		bool end_added;

		// function to call at the end of every thread
		function<void(int)> *end_callback;

		// vector to contain the failed urls
		Channel<failed_url>* chan_failed_urls;

	}; /*class DepthPoolManager*/

} /*namespace webler*/

#endif /*__WEBLER_DEPTH_MANAGER_HPP__*/