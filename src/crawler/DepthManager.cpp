#include <crawler/DepthHandler.hpp>
#include <crawler/DepthManager.hpp>
#include <crawler/http.hpp>
#include <iostream>
using namespace std;


/*#####*/
void web::__DepthPoolThreadFunction(string regex_str,web_chan_ptr chanGet,web_chan_ptr chanPut, function<void(web_chan_ptr,int)> callback, int rpd_id, bool is_end){

	web::DepthHandler(
		regex_str,
		chanGet,
		chanPut,
		is_end
	).start();

	// end callback when DepthHandler has finished
	callback(chanGet,rpd_id);

}

/*######### DepthPoolManager #########*/
web::DepthPoolManager::DepthPoolManager(int pool_size, int t_count) {

	max_pool_size = pool_size;
	thread_count = t_count;	
	rp_count = 0;
	end_added = false;

	end_callback = new function<void(web_chan_ptr,int)>;
	*end_callback = 
	[this] // captures
	(web_chan_ptr chanGet, int rpd_id)-> void {

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
							web::wait_pool_data wpd = this->wait_pool.front();

							web::running_pool_data rpd;
							this->rp_count++;
							rpd._id = this->rp_count;
							rpd.chan_put = wpd.chan_put;
							rpd.threads = new running_pool_thread_data[this->thread_count];

							// creating threads for new entry in running pool
							for(int i=0; i<this->thread_count; i++) {
								rpd.threads[i]._t = thread(
									web::__DepthPoolThreadFunction,
									wpd.regex_str,
									wpd.chan_get,
									wpd.chan_put,
									(*this->end_callback),
									rpd._id,
									wpd.is_end);
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

void web::DepthPoolManager::add_depth(string regex_str, web_chan_ptr chanGet, web_chan_ptr chanPut, bool is_end) {
	if (end_added) return;
	
	pool_mutex.lock();
	if (is_end) end_added = true;

	if(running_pool.size()<max_pool_size) {
		// add in running queue (running pool is not full)

		// data to put in running pool
		web::running_pool_data rpd;
		rp_count++;
		rpd._id = rp_count;
		rpd.chan_put = chanPut;
		rpd.threads = new running_pool_thread_data[thread_count];

		// creating threads in running pool
		for(int i=0; i<thread_count; i++) {
			rpd.threads[i]._t = thread(
				web::__DepthPoolThreadFunction,
				regex_str,
				chanGet,
				chanPut,
				(*end_callback),
				rpd._id,
				is_end);
			rpd.threads[i].is_active = true;
		}

		rpd.threads_active = thread_count;
		running_pool.push_back(rpd);
		pool_mutex.unlock();


	} else {
		// add in waiting pool (running pool is full)
		wait_pool.push_back(web::wait_pool_data(regex_str,chanGet,chanPut,is_end));
		pool_mutex.unlock();
	}

}
