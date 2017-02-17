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
		vector<string> regexes_str;
		void (*callback)(bool,string,string);
		bool in_progress;

	public:
		WebCrawler();
		bool set_options(string, int, vector<string>);
		void set_callback(void (*)(bool, string, string));
		bool is_in_progress();

		bool start(); // TODO
		bool stop(); // TODO

	};

}

#endif