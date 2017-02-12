#include <vector>
#include <deque>
#include <string>
#include <web/channel.hpp>
#include <web/depth_manager.hpp>
#include <web/http.hpp>
using namespace std;

void web::DepthPoolThreadFunction(string regex_str,web::Channel<channel_data>* chanGet,web::Channel<channel_data>* chan_put, void (*callback)()){

}

/*######### DepthHandler #########*/
web::DepthHandler::DepthHandler(string regex_str, web::Channel<channel_data>* chanGet, web::Channel<channel_data>* chanPut, void (*callback)()){

}

void web::DepthHandler::start() {

}

/*######### DepthPoolManager #########*/
web::DepthPoolManager::DepthPoolManager(int pool_size, int thread_count) {

}

void web::DepthPoolManager::add_new_depth(string regex_str, web::Channel<channel_data>* chanGet, web::Channel<channel_data>* chanPut) {

}