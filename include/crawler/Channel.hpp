#ifndef __WEBLER_CHANNEL_HPP__

#define __WEBLER_CHANNEL_HPP__

#include <queue>
#include <mutex>
#include <atomic>
#include <semaphore.h>
using namespace std;

namespace webler {

    const int CHAN_SUCCESS = 0;
    const int CHAN_CLOSED = -1;
    
    /*
    * Class template to send and receive data
    **/
    template <class T>
    class Channel {

    public:
        // Constructor
        Channel();

        /*
        * @Params: (Data to add in channel queue)
        * returns 'false' if channel is closed, else true
        **/
        int add(const T data);

        /*
        * @Params: (bool variable to be set to true if channel is closed else false)
        * returns data from the queue of added elements. 
        * If channel is closed, it returns empty element.
        * Note: Check if the channel is closed from the variable passed. 
        *         Data returned is invalid if channel is closed.
        **/
        T retrieve(bool *closed);

        /*
        * @Params: None
        * Used to close the channel
        **/
        int close();

    private:
        
        // buffer to store incoming data
        queue<T> buffer;

        // true if channel is closed
        bool chan_closed;

        // to lock operations on buffer queue
        mutex buffer_mutex;

        // to lock operations on chan_queue
        mutex close_mutex;

        // to check if queue is empty 
        sem_t buffer_sem;

        // number of retrieve calls waiting for data
        atomic<int> waiting_length; 

    }; /*class Channel*/


} /*namespace webler*/

template <class T>
webler::Channel<T>::Channel() {
    sem_init(&buffer_sem, 0, 0);
    waiting_length = 0;
    chan_closed = false;
}

template <class T>
int webler::Channel<T>::add(const T data) {

    unique_lock<mutex> closelock(close_mutex);

    if(!chan_closed) {
        unique_lock<mutex> bufferlock(buffer_mutex);
        buffer.push(data);
        bufferlock.unlock();
        closelock.unlock();
        
        sem_post(&buffer_sem);
        return webler::CHAN_SUCCESS;
    } else {
        closelock.unlock();
        return webler::CHAN_CLOSED;
    }

}

template <class T>
T webler::Channel<T>::retrieve(bool *closed) {

    unique_lock<mutex> closelock(close_mutex);
    if(!chan_closed) {
        
        ++waiting_length;
        closelock.unlock();

        sem_wait(&buffer_sem);
        --waiting_length;

    } else if(buffer.size()==0) {
        closelock.unlock();
        *closed = true;
        T temp;
        return temp;
    } else {
        closelock.unlock();
    }

    unique_lock<mutex> bufferlock(buffer_mutex);

    if(buffer.size()>0) {
        T data = buffer.front();
        buffer.pop();
        bufferlock.unlock();
        *closed = false;
        return data;
    } else {
        bufferlock.unlock();
        *closed = true;
        T temp;
        return temp;
    }

}

template <class T>
int webler::Channel<T>::close() {

    unique_lock<mutex> closelock(close_mutex);
    if(!chan_closed){
        chan_closed = true;
        int current_waiting = waiting_length;
        while(--current_waiting >= 0) {
            sem_post(&buffer_sem);
        }
        return webler::CHAN_SUCCESS;
    } else {
        return webler::CHAN_CLOSED;
    }

}

#endif /*__WEBLER_CHANNEL_HPP__*/