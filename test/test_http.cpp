#include <iostream>
#include <webler/http.hpp>
using namespace std;

int main () {

	web::http_response r;
	// r = web::http_get("http://localhost:3000/first/");
	r = web::http_get("http://example.com");
	if(r.success) {
		cout << r.html << endl;
	} else {
		cout << "Error occured" << endl;		
	}

	cout << "HTTP GET test is over" << endl;

	return 0;
}