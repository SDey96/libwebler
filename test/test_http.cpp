#include <iostream>
#include <web/http.hpp>
using namespace std;

int main () {

	web::http_response r;
	r = web::http_get("http://example.com");
	if(r.success) {
		cout << r.html << endl;
	} else {
		cout << "Error occured" << endl;		
	}

	return 0;
}