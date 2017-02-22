#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <web/WebCrawler.hpp>
using namespace std;

void callback(bool status, string url, vector<string> data) {


	cout << endl;
	if(status) cout << "YES" << endl;
	else cout << "NO" << endl;
	cout << "URL: " << url << endl;
	for(auto i: data) {
		cout << i << endl;
	}
	cout << endl;

}

int main () {

	web::WebCrawler test_crawler;

	vector<string> re = {
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>"),
		string("<\\s*ul\\s+[^>]*id\\s*=\\s*\"junkData\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*div\\s+[^>]*id\\s*=\\s*\"wantedData\"[^>]*>([^?]*(?=</div>))</div>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"junkData2\"[^>]*>[^?]*(?=</ul>)</ul>")
	};

	if(!test_crawler.set_metadata(
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
	test_crawler.start();


	return 0;
}