#ifndef __WEBLER_CRAWLER_HPP__

#define __WEBLER_CRAWLER_HPP__

#include <string>
#include <vector>
using namespace std;

namespace web {

	const int WC_SUCCESS = 0;
	const int WC_EINVALID = -1;
	const int WC_EUNKNOWN = -2;
	const int WC_EINPROGRESS = -3;

	#ifndef __WEBLER_FAILED_URL__
	#define __WEBLER_FAILED_URL__
	struct failed_url {
		string url;
		int depth;
		failed_url() {}
		failed_url(string u, int d): url(u), depth(d) {}
	};
	#endif
	
	/*
	* Main class to start web crawling
	**/
	class WebCrawler {

	public:
		// Constructor
		WebCrawler();

		/*
		* @Params: (first URL, total depth, vector having regex for each depth)
		* returns 'false' if in progress or wrong data, else 'true'
		**/
		int set_basedata(string _root_url, int _depth, vector<string> _regexes_str);

		/*
		* @Params: (max_depth, depth_threads)
		* returns 'false' if in progress, else 'true'
		**/
		int set_concurrency_options(int _max_depth, int _depth_threads);

		/*
		* @Params: Function of type 'void (bool status, string url, vector<string> data)'
		*		status: 'true' if successful, else false
		*		url: URL in the last depth crawled
		*		data: vector of captures in url using the regex for last depth
		*
		* returns 'false' if in progress, else 'true'
		**/
		int set_callback(void (*_callback)(bool, string, vector<string>));

		/*
		* @Params: None
		* returns 'true' if in progress, else 'false'
		**/
		bool is_in_progress();

		/*
		* @Params: None
		* returns 'false' if in progress, else 'true' after crawling is finished
		* Note: Should be used after successfully calling set_basedata()
		**/
		int start();

		/*
		* @Params: None
		* returns a vector<failed_url> containing all failed URL
		**/
		vector<failed_url> get_failed_url();

	private:
		// URL to start with
		string root_url;

		// total depth to crawl (including first URL)
		int depth;

		// Number of threads to create in a depth
		int depth_threads;

		// Max number of depths crawling at any given time
		int max_depth;

		// Regex to parse HTML at every depth
		vector<string> regexes_str;

		// Called with the data in the last depth
		void (*callback)(bool,string,vector<string>);

		// true if crawling has started
		bool in_progress;

		// true if minimum information required is updated (set_basedata() was successful)
		bool basedata_updated;

		// vector containing all failed URLs
		vector<failed_url> failed_urls;
	};

	// to extract href from <a> tags of html
	vector<string> get_href(string html);

}

#endif