#include <iostream>
#include "/usr/local/lib/web/http.hpp"
#include "/usr/local/lib/web/crawler.hpp"
using namespace std;

int main () {
	web::http_response r;
	r = web::http_get("http://example.com");

	cout << r.html << endl;

	return 0;
}