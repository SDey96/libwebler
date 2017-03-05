#include <crawler/DepthHandler.hpp>
#include <crawler/DepthManager.hpp>
using namespace std;


void webler::__DepthPoolThreadFunction(
	string regex_str,
	web_chan_ptr chan_get,
	web_chan_ptr chan_put, 
	function<void(int)> callback, 
	int rpd_id, 
	bool is_end,
	Channel<failed_url>* chan_failed_url ) {

	webler::DepthHandler(
		rpd_id,
		regex_str,
		chan_get,
		chan_put,
		is_end,
		chan_failed_url
	).start();

	// end callback when DepthHandler has finished
	return callback(rpd_id);

}

/*######### DepthPoolManager #########*/
webler::DepthPoolManager::DepthPoolManager(int _max_pool_size, int _thread_count, Channel<failed_url>* _failed_urls) {

	max_pool_size = _max_pool_size;
	thread_count = _thread_count;
	chan_failed_urls = _failed_urls;	
	rp_count = 0;
	end_added = false;

	end_callback = new function<void(int)>;
	*end_callback = 
	[this] // captures
	(int rpd_id)-> void {

		this->pool_mutex.lock();
		int pool_size = this->running_pool.size();
			
		for(int i=0; i<pool_size; i++) { // checking which thread called
			
			if(this->running_pool[i]._id == rpd_id) { // thread detected

				this->running_pool[i].threads_active--;
			
				if(i==0){ // thread is first in the pool queue
			
					while(running_pool.size()>0 && this->running_pool[0].threads_active<=0){
						// removing finished threads groups from head of pool

						this->running_pool[0].chan_put->close();
						this->running_pool.pop_front();
			
						if(wait_pool.size()>0) { // if wait pool is waiting to run

							// creating running pool data from waiting pool
							webler::wait_pool_data wpd = this->wait_pool.front();

							webler::running_pool_data rpd;
							this->rp_count++;
							rpd._id = this->rp_count;
							rpd.chan_put = wpd.chan_put;
							rpd.threads = new running_pool_thread_data[this->thread_count];

							// creating threads for new entry in running pool
							for(int i=0; i<this->thread_count; i++) {
								rpd.threads[i]._t = thread(
									webler::__DepthPoolThreadFunction,
									wpd.regex_str,
									wpd.chan_get,
									wpd.chan_put,
									(*this->end_callback),
									rpd._id,
									wpd.is_end,
									this->chan_failed_urls);
								rpd.threads[i].is_active = true;
							}

							rpd.threads_active = this->thread_count;
							this->running_pool.push_back(rpd);
							this->wait_pool.pop_front();
						}
			
					}
			
				}

				break;
			}

		}

		this->pool_mutex.unlock();
	};

}

webler::DepthPoolManager::~DepthPoolManager() {
	if(end_callback){
		delete end_callback;
	}
}

bool webler::DepthPoolManager::add_depth(string regex_str, web_chan_ptr _chan_get, web_chan_ptr _chan_put, bool is_end) {
	if (end_added) return false;
	
	pool_mutex.lock();
	if (is_end) end_added = true;

	if(running_pool.size()<max_pool_size) {
		// add in running queue (running pool is not full)

		// data to put in running pool
		webler::running_pool_data rpd;
		rp_count++;
		rpd._id = rp_count;
		rpd.chan_put = _chan_put;
		rpd.threads = new running_pool_thread_data[thread_count];

		// creating threads in running pool
		for(int i=0; i<thread_count; i++) {
			rpd.threads[i]._t = thread(
				webler::__DepthPoolThreadFunction,
				regex_str,
				_chan_get,
				_chan_put,
				(*end_callback),
				rpd._id,
				is_end,
				chan_failed_urls);
			rpd.threads[i].is_active = true;
		}

		rpd.threads_active = thread_count;
		running_pool.push_back(rpd);
		pool_mutex.unlock();


	} else {
		// add in waiting pool (running pool is full)
		wait_pool.push_back(webler::wait_pool_data(regex_str,_chan_get,_chan_put,is_end));
		pool_mutex.unlock();
	}

	return true;

}