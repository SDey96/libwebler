#ifndef __WEB_CHANNEL__

#define __WEB_CHANNEL__

#include <queue>
#include <mutex>
#include <iostream>
#include <semaphore.h>
using namespace std;

namespace web {
	
	template <class T>
	class Channel {

	private:
		mutex buffer_mutex;
		sem_t buffer_sem;
		queue<T> buffer;

	public:
		Channel(){
			sem_init(&buffer_sem, 0, 0);
		}
		~Channel(){}

		void add(const T data) ;

		T retrive();

	};


}

template <class T>
void web::Channel<T>::add(const T data) {

	buffer_mutex.lock();
	buffer.push(data);
	buffer_mutex.unlock();
	
	sem_post(&buffer_sem);

}

template <class T>
T web::Channel<T>::retrive() {

	sem_wait(&buffer_sem);
	
	T data = buffer.front();
	buffer_mutex.lock();
	buffer.pop();
	buffer_mutex.unlock();

	return data;

}

#endif