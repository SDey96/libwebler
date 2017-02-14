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
		Channel();
		~Channel(){}

		void add(const T data) ;
		T retrieve(bool*);

		void close();
		bool is_closed();

	};


}

template <class T>
web::Channel<T>::Channel() {
	sem_init(&buffer_sem, 0, 0);
	chan_closed = false;
}

template <class T>
void web::Channel<T>::add(const T data) {

	buffer_mutex.lock();
	buffer.push(data);
	buffer_mutex.unlock();
	
	sem_post(&buffer_sem);

}

template <class T>
T web::Channel<T>::retrieve(bool *closed) {
	if(chan_closed){
		*closed = true;
		T temp;
		return temp;
	} else {
		sem_wait(&buffer_sem);

		if(chan_closed){
			*closed = true;
			T temp;
			return temp;
		} else {
			*closed = false;
			buffer_mutex.lock();
			T data = buffer.front();
			buffer.pop();
			buffer_mutex.unlock();
			return data;
		}
	}
}

template <class T>
void web::Channel<T>::close() {
	chan_closed = true;
}

#endif