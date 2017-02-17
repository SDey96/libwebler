#include <iostream>
#include <string>
#include <thread>
#include <web/channel.hpp>
using namespace std;

#define N 100

void thread_function(int i,web::Channel<int> *chan) {
	for(int j=0; j<N; j++) {
		chan->add(i);
	}
}

void thread_function2(int i,web::Channel<int> *chan) {
	if(i==5) {
		chan->close();
	}
	for(int j=0; j<N; j++) {
		chan->add(i);
	}
}


int main () {

	web::Channel<int> *chan = new web::Channel<int>;

	int total_threads = 10;

	thread *thread_array = new thread[total_threads];

	int t = total_threads*N;
	int *arr = new int[total_threads]();
	bool closed;

	// ##### uncomment any one section #####
	
	// #####
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
	// #####

	// #####
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
	// #####

	return 0;
}