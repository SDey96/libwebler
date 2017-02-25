#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <webler/WebCrawler.hpp>
#include <ctime>
using namespace std;

int counter = 0;

void callback(bool status, string url, vector<string> data) {


	cout << endl;
	if(status) cout << "Received " << (++counter) << endl;
	else cout << "NO" << endl;
	cout << "URL: " << url << endl;
	cout << "Data: ";
	for(auto i: data) {
		cout << i << endl;
	}
	cout << endl;

}

int main () {

	web::WebCrawler test_crawler;

	string regex1("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>");
	string regex2("<\\s*ul\\s+[^>]*id\\s*=\\s*\"junkData\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*div\\s+[^>]*id\\s*=\\s*\"wantedData\"[^>]*>([^?]*(?=</div>))</div>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"junkData2\"[^>]*>[^?]*(?=</ul>)</ul>");

	vector<string> re = { regex1, regex1, regex1, regex1, regex1, regex1, regex1, regex1, regex1, regex1, regex2 };

	if(!test_crawler.set_basedata(
		string("http://localhost:3000/first/"),
		11,
		re
	)) {
		cout << "Error in setting metadata" << endl;
		return 0;
	}

	if(!test_crawler.set_concurrency_options(3,5)) {
		cout << "Error in setting concurrency options" << endl;
		return 0;
	}

	if(!test_crawler.set_callback(callback)) {
		cout << "Error in setting callback" << endl;
		return 0;
	}

	timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
	test_crawler.start();
    clock_gettime(CLOCK_MONOTONIC,&end_time);

    vector<web::failed_url> fu = test_crawler.get_failed_url();
    for(auto i: fu) {
    	cout << "Failed:: Depth:" << i.depth << ", URL:" << i.url << endl; 
    }

    cout << "Time taken: " << end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec)/1e09 << endl;


	return 0;
}