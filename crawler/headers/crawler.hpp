#ifndef __WEB_CRAWLER__

#define __WEB_CRAWLER__

#include <web/http.hpp>
#include <string>
#include <vector>
#include <exception>
using namespace std;

namespace web {
	
	class WebCrawler {

	private:
		string root_url;
		int depth;
		vector<string> regexes_str;
		void (*callback)(bool,string,string);

	public:
		WebCrawler();
		bool set_options(string, int, vector<string>);
		void set_callback(void (*)(bool, string, string));

	};

}

#endif