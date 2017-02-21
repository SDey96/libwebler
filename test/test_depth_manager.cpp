#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <web/Channel.hpp>
#include <web/DepthManager.hpp>
using namespace std;


int main () {

	web::Channel<web::channel_data> *chan_get = new web::Channel<web::channel_data>;
	web::Channel<web::channel_data> *chan_put = new web::Channel<web::channel_data>;
	
	web::channel_data c_get_data, c_res_data;
	
	c_get_data.links.push_back(string("http://timesofindia.indiatimes.com/2016/1/1/archivelist/year-2016,month-1,starttime-42370.cms"));
	chan_get->add(c_get_data);

	web::DepthPoolManager dpm(3,2);
	dpm.add_depth(string("(.*?)"),chan_get,chan_put,true);

	bool closed = false;
	cout << "Waiting to retrieve ..." << endl;
	c_res_data = chan_put->retrieve(&closed);
	chan_get->close();
	while(!closed){
		chan_put->retrieve(&closed);
	}
	cout << "Retrieved ..." << endl;
	
	for(vector<string>::iterator it=c_res_data.links.begin(); it!=c_res_data.links.end(); ++it ) {
    	cout << *it << endl;
	}

	cout << "DepthPoolManager test is over" << endl;
	
	return 0;
}