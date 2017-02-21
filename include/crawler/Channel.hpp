#ifndef __WEB_CHANNEL__

#define __WEB_CHANNEL__

#include <queue>
#include <mutex>
#include <atomic>
#include <semaphore.h>
using namespace std;

namespace web {
	
	template <class T>
	class Channel {

	private:
		
		queue<T> buffer; // buffer to store incoming data
		bool chan_closed; // true if channel is closed

		mutex buffer_mutex; // to lock operations on buffer queue
		mutex close_mutex; // to lock operations on chan_queue
		sem_t buffer_sem; // to keep track of data in queue

		atomic<int> waiting_length; // number of retrieve calls waiting for data

	public:
		Channel();
		~Channel(){}

		int add(const T data) ; // to put data into the channel
		T retrieve(bool*); // to get data from channel

		void close(); // to close recieving of data (channel closed)

	};


}

template <class T>
web::Channel<T>::Channel() {
	sem_init(&buffer_sem, 0, 0);
	waiting_length = 0;
	chan_closed = false;
}

template <class T>
int web::Channel<T>::add(const T data) {

	close_mutex.lock();
	if(!chan_closed) {
		buffer_mutex.lock();
		buffer.push(data);
		buffer_mutex.unlock();
		close_mutex.unlock();
		
		sem_post(&buffer_sem);
		return 0;
	} else {
		close_mutex.unlock();
		return -1;
	}

}

template <class T>
T web::Channel<T>::retrieve(bool *closed) {

	close_mutex.lock();
	if(!chan_closed) {
		
		++waiting_length;
		close_mutex.unlock();

		sem_wait(&buffer_sem);
		--waiting_length;

	} else if(buffer.size()==0) {
		close_mutex.unlock();
		*closed = true;
		T temp;
		return temp;
	} else {
		close_mutex.unlock();
	}

	buffer_mutex.lock();

	if(buffer.size()>0) {
		T data = buffer.front();
		buffer.pop();
		buffer_mutex.unlock();
		*closed = false;
		return data;
	} else {
		buffer_mutex.unlock();
		*closed = true;
		T temp;
		return temp;
	}

}

template <class T>
void web::Channel<T>::close() {

	close_mutex.lock();
	if(!chan_closed){
		chan_closed = true;
		int current_waiting = waiting_length;
		while(--current_waiting >= 0) {
			sem_post(&buffer_sem);
		}
	}
	close_mutex.unlock();

}

#endif