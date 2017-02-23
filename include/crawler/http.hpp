#ifndef __WEBLER_HTTP__

#define __WEBLER_HTTP__

#include <string>
using namespace std;
namespace web {

	struct http_response {
		string html; // response HTML
		bool success; // true if GET request was successful
		http_response(): success(true), html("") {}
	};

	/*
	* @Params: (URL)
	* returns http_response datatype
	**/
	http_response http_get(string url);
}

#endif