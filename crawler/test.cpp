#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <web/http.hpp>
#include <web/crawler.hpp>
#include <web/channel.hpp>
#include <web/depth_manager.hpp>
using namespace std;

// FOR CHANNEL TEST
// #define N 100

// void thread_function(int i,web::Channel<int> *chan) {
// 	for(int j=0; j<N; j++) {
// 		chan->add(i);
// 	}
// }

// void thread_function2(int i,web::Channel<int> *chan) {
// 	if(i==5) {
// 		chan->close();
// 	}
// 	for(int j=0; j<N; j++) {
// 		chan->add(i);
// 	}
// }


// FOR DepthHandler // TODO: update with proper regex to test
// void DH_function(web::Channel<web::channel_data> *chanGet,web::Channel<web::channel_data> *chanPut) {
// 	web::DepthHandler DH(
// 		string("(.*?)"),
// 		chanGet,
// 		chanPut,
// 		NULL
// 	);

// 	DH.start();
// 	cout << "DepthHandler test is over" << endl;
// }

int main () {

	// FOR CHANNEL TEST
	// web::Channel<int> *chan = new web::Channel<int>;

	// int total_threads = 10;

	// thread *thread_array = new thread[total_threads];

	// int t = total_threads*N;
	// int *arr = new int[total_threads]();
	// bool closed;

	
	// CHANNEL TEST: Normal working of channel
	// for(int j=0; j<total_threads; j++){
	// 	thread_array[j] = thread(thread_function,j,chan);
	// }
	
	// for(int j=0; j<t; j++) {
	// 	arr[chan->retrieve(&closed)]++;
	// }
	
	// for(int j=0; j<total_threads; j++) {
	// 	cout << arr[j] << endl;
	// }

	// CHANNEL TEST: Testing closing feature of channel
	// for(int j=0; j<total_threads; j++){
	// 	thread_array[j] = thread(thread_function2,j,chan);
	// }

	// for(int j=chan->retrieve(&closed); !closed ; j=chan->retrieve(&closed)) {
	// 	arr[j]++;
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



	// DepthHandler TEST
	// web::Channel<web::channel_data> *chan_get = new web::Channel<web::channel_data>;
	// web::Channel<web::channel_data> *chan_put = new web::Channel<web::channel_data>;
	
	// web::channel_data c_get_data, c_res_data;
	
	// c_get_data.links.push_back(string("http://timesofindia.indiatimes.com/2016/1/1/archivelist/year-2016,month-1,starttime-42370.cms"));
	// chan_get->add(c_get_data);

	// thread t = thread(DH_function,chan_get,chan_put);
	
	// bool closed;
	// cout << "Waiting to retrieve" << endl;
	// c_res_data = chan_put->retrieve(&closed);
	// cout << "Retrieved" << endl;
	// if(closed){
	// 	cout << "Channel is closed" << endl;
	// } else {
	// 	for(vector<string>::iterator it=c_res_data.links.begin(); it!=c_res_data.links.end(); ++it ) {
 //    		cout << *it << endl;
	// 	}
	// }

	// t.join();

	return 0;
}