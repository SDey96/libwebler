#ifndef __WEB_CHANNEL__

#define __WEB_CHANNEL__

#include <queue>
#include <mutex>
#include <semaphore.h>
using namespace std;

namespace web {
	
	template <class T>
	class Channel {

	private:
		mutex buffer_mutex;
		sem_t buffer_sem;
		queue<T> buffer;
		bool chan_closed;

	public:
		Channel(){
			sem_init(&buffer_sem, 0, 0);
		}
		~Channel(){}

		void add(const T data) ;

		T retrieve();

		void close();

		bool is_closed();

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
T web::Channel<T>::retrieve() {

	sem_wait(&buffer_sem);
	
	buffer_mutex.lock();
	T data = buffer.front();
	buffer.pop();
	buffer_mutex.unlock();

	return data;

}

// TODO: synchronize close() and is_closed()
template <class T>
void web::Channel<T>::close() {
	chan_closed = true;
}

template <class T>
bool web::Channel<T>::is_closed() {
	return chan_closed;
}

#endif