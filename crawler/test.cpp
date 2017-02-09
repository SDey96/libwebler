#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <web/http.hpp>
#include <web/crawler.hpp>
#include <web/channel.hpp>
using namespace std;

// FOR CHANNEL TEST
// #define N 100

// void thread_function(int i,web::Channel<int> *chan) {
// 	for(int j=0; j<N; j++) {
// 		chan->add(i);
// 	}
// }

// FOR CRAWLER TEST
// void callback(bool status, string url, string html) {
// 	if(status) {
// 		cout << url << endl;
// 		cout << html << endl;
// 	} else {
// 		cout << "Error with " << url << endl;
// 	}
// }

int main () {

	// CHANNEL TEST
	// web::Channel<int> *chan = new web::Channel<int>;

	// int total_threads = 100;

	// thread *thread_array = new thread[total_threads];

	// for(int j=0; j<total_threads; j++){
	// 	thread_array[j] = thread(thread_function,j,chan);
	// }

	// int t = total_threads*N;
	// int *arr = new int[total_threads];
	// for(int j=0; j<t; j++) {
	// 	arr[chan->retrive()]++;
	// }
	
	// for(int j=0; j<total_threads; j++) {
	// 	cout << arr[j] << endl;
	// }


	// HTTP GET TEST
	// web::http_response r;
	// r = web::http_get("http://example.com");
	// if(r.success) {
	// 	cout << r.html << endl;
	// } else {
	// 	cout << "Error occured" << endl;		
	// }

	// CRAWLER TEST
	// web::WebCrawler WC;
	// if(!WC.set_options("http://example.com",2,{string("regex1"),string("regex2")})) {
	// 	cout << "Error in setting options" << endl;
	// }
	// WC.set_callback(callback);

	return 0;
}