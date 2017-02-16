#include <web/depth_manager.hpp>
#include <web/http.hpp>
#include <iostream>
using namespace std;

/*######### DepthHandler #########*/
web::DepthHandler::DepthHandler(string rx_str, web_chan chanGet, web_chan chanPut){

	regex_str = rx_str;
	chan_get = chanGet;
	chan_put = chanPut;

}

void web::DepthHandler::start() {

	vector<string> channel_urls,extracted_urls;

	// to match links
	regex link_extractor("<\\s*a\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", regex::icase);
	
	// to match html
	regex html_extractor(regex_str.c_str(), regex::icase);
	smatch html_match;

	// response from GET request
	web::http_response res;

	// HTML after filtering with regex
	string filtered_html;

	int match_length;

	web::channel_data send_data;

	bool closed;

	// getting urls from channel
	for(web::channel_data it=chan_get->retrieve(&closed); !closed ; it=chan_get->retrieve(&closed)) {
		// link got from channel
		channel_urls = it.links;

		// iterating over the links
		for(vector<string>::iterator url=channel_urls.begin(); url!=channel_urls.end(); ++url ) {
			
			// GET requests to the link
			res = web::http_get(*url);

			if(res.success) { // GET request was successful

				// filtering the HTML
				if( regex_search(res.html, html_match, html_extractor) && html_match.size()>1 ){
					
					match_length = html_match.size();
					filtered_html = "";

					for(int i=1; i<=match_length; i++) {
						filtered_html += html_match.str(i);
					}

					// extracting URL
					extracted_urls.clear();
					copy(
					    sregex_token_iterator( filtered_html.begin(), filtered_html.end(), link_extractor, 1 ),
					    sregex_token_iterator(),
					    back_inserter(extracted_urls)
				    );

				    send_data.links = extracted_urls;
				    chan_put->add(send_data);
					
				}

			} else {
				// TODO handle faliure
				cout << "Fail: " << *url << endl;
			}

		}

	}
}

/*#####*/
void web::__DepthPoolThreadFunction(string regex_str,web_chan chanGet,web_chan chanPut, function<void(web_chan,int)> callback, int rpd_id){

	web::DepthHandler(
		regex_str,
		chanGet,
		chanPut
	).start();

	// end callback when DepthHandler has finished
	callback(chanGet,rpd_id);

}

/*######### DepthPoolManager #########*/
web::DepthPoolManager::DepthPoolManager(int pool_size, int thread_count) {

	max_pool_size = pool_size;
	thread_count = thread_count;	
	rp_count = 0;

}

void web::DepthPoolManager::add_new_depth(string regex_str, web_chan chanGet, web_chan chanPut) {
	pool_mutex.lock();
	if(running_pool.size()<max_pool_size) {
		// add in running queue (running pool is not full)

		// data to put in running pool
		web::running_pool_data rpd;
		rp_count++;
		rpd._id = rp_count;
		rpd.chan_get = chanGet;
		rpd.threads = new thread[thread_count];

		// function to run after each thread is over
		function<void(web_chan,int)> end_callback = 
		[this,end_callback] // captures
		(web_chan chanGet, int rpd_id)-> void {

			this->pool_mutex.lock();
			int pool_size = this->running_pool.size();
			
			for(int i=0; i<pool_size; i++) { // checking which thread called
			
				if(this->running_pool[i]._id == rpd_id) { // thread detected

					this->running_pool[i].threads_active--;
			
					if(i==0){ // thread is first in the pool queue
			
						while(running_pool.size()>0 && this->running_pool[i].threads_active<=0){
							// removing finished threads groups from head of pool

							delete this->running_pool[i].chan_get; // deallocating channel which wont be used
							this->running_pool.pop_front();
			
							if(wait_pool.size()>0) { // if wait pool is waiting to run

								// creating running pool data from waiting pool
								web::wait_pool_data wpd = this->wait_pool.front();

								web::running_pool_data rpd;
								this->rp_count++;
								rpd._id = this->rp_count;
								rpd.chan_get = wpd.chan_get;
								rpd.threads = new thread[this->thread_count];

								// creating threads for new entry in running pool
								for(int i=0; i<this->thread_count; i++) {
									rpd.threads[i] = thread(
										web::__DepthPoolThreadFunction,
										wpd.regex_str,
										wpd.chan_get,
										wpd.chan_put,
										end_callback,
										rpd._id);
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

		// creating threads in running pool
		for(int i=0; i<thread_count; i++) {
			rpd.threads[i] = thread(
				web::__DepthPoolThreadFunction,
				regex_str,
				chanGet,
				chanPut,
				end_callback,
				rpd._id);
		}

		rpd.threads_active = thread_count;
		running_pool.push_back(rpd);
		pool_mutex.unlock();

	} else {
		// add in waiting pool (running pool is full)
		wait_pool.push_back(web::wait_pool_data(regex_str,chanGet,chanPut));
		pool_mutex.unlock();
	}

}