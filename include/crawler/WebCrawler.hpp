#ifndef __WEB_CRAWLER__

#define __WEB_CRAWLER__

#include <web/http.hpp>
#include <string>
#include <vector>
using namespace std;

namespace web {
	
	class WebCrawler {

	private:
		string root_url;
		int depth;
		int depth_threads;
		int max_depth;
		vector<string> regexes_str;
		void (*callback)(bool,string,vector<string>);
		bool in_progress;
		bool metadata_updated;

	public:
		WebCrawler();
		bool set_metadata(string, int, vector<string>);
		bool set_concurrency_options(int, int);
		bool set_callback(void (*)(bool, string, vector<string>));
		bool is_in_progress();

		bool start();

	};

}

#endif