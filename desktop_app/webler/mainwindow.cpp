#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <vector>
#include <regex>
#include <QInputDialog>
#include <QString>
#include <QDir>
#include <webler/WebCrawler.hpp>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctime>
using namespace std;

// home directory of the system
string homedir = "";

/* Web Crawler */

// true if the crawler has started
bool crawler_started = false;

// total logs in web crawler
int web_crawler_log_count = 0;

// While in progress
// Number of callbacks from web crawler
int counter = 0;

// Main webler directory for web crawler
string weblerdir;

// Current web crawler ID (folder name to store data)
string sessionid;

/* [end] Web Crawler */

// to update session id for new web crawler session
void update_sessionid() {

    time_t now = time(0);
    tm *ltm = localtime(&now);

    // Current time in session id
    sessionid = to_string(1900 + ltm->tm_year) + to_string(1 + ltm->tm_mon) + to_string(ltm->tm_mday) + to_string(1 + ltm->tm_hour) + to_string(1 + ltm->tm_min) + to_string(1 + ltm->tm_sec);

    // creating new directory for the current session files
    weblerdir = homedir + "/webler/" + sessionid;
    QDir().mkdir(weblerdir.c_str());
}

string get_file_dir(int x) {
    return weblerdir + "/result_" + to_string(x) + ".txt";
}

// callback used in WebCrawler
void callback(string url, vector<string> data) {

    counter++;

    // writing data in a file
    ofstream outfile;
    string filedir = get_file_dir(counter);
    outfile.open(filedir.c_str());

    // URL in the file
    outfile << "URL: " << url << endl;

    // data of last depth in the file
    outfile << "Data: " << endl;;
    for(auto i: data) {
        outfile << i << endl << endl;
    }
    cout << filedir << endl;

    outfile.close();

}

// Window constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->messages->setReadOnly(true);
    
    web_crawler_logs = "";
    _baseURL = "";
    _depth = 0;

    // getting the home directiry
    if (getenv("HOME") == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    } else {
        homedir = getenv("HOME");
    }

    QDir().mkdir((homedir+"/webler").c_str());

}

// Window destructor
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {}

// updating Base URL
void MainWindow::on_lineEdit_textEdited(const QString &arg1) {
    _baseURL = arg1.toUtf8().constData();
}

// to update the logs with new message
void MainWindow::update_message(string msg) {
    // adding new msg to current msg
    web_crawler_logs = "[" + to_string(++web_crawler_log_count) + "] " + msg + "\n" + web_crawler_logs;
    // updating logs in UI
    ui->messages->document()->setPlainText(web_crawler_logs.c_str());
}

// to get depth and regex
void MainWindow::on_getDepth_clicked() {

    // getting the depth
    _depth = QInputDialog::getInt(this,tr("Enter Depth"),tr("Depth"));
 
    if(_depth > 0) {
        // valid depth

        ui->depthValue->setText(QString::number(_depth));
        update_message("Depth set to " + to_string(_depth));
        _regexString.clear();
        update_message("Regex vector cleared");

        // getting regexes
        for(int i=0; i<_depth; i++) {
            _regexString.push_back( QInputDialog::getText(this, 
                tr("Regex"),
                tr(("Enter regex for depth " + to_string(i+1) + "/" + to_string(_depth) + ":").c_str()), 
                QLineEdit::Normal).toUtf8().constData() );
            update_message("Regex " + to_string(i+1) + " set to " + _regexString.back());
        }

    } else {
        // Invalid depth
        update_message("Invalid depth " + to_string(_depth));
        _depth = 0;
        ui->depthValue->setText(QString::number(_depth));
    }

}

// to start the crawler
void MainWindow::on_submit_clicked() {

    counter = 0;

    if(crawler_started){
        update_message("Crawling is in progress");
        return;
    }

    if(_baseURL == "") {
        update_message("Error: Invalid base URL");
        return;
    }

    if(_depth <= 0) {
        update_message("Error: Invalid depth");
        return;
    }

    if(_depth != _regexString.size()) {
        update_message("Error: Invalid number of regex");
        return;
    }

    ui->submit->setEnabled(false);

    crawler_started = true;
    update_sessionid();
    webler::WebCrawler _crawler;

    // setting base data for the crawler
    if(_crawler.set_basedata(
        _baseURL,
        _depth,
       _regexString
       // re
    ) != webler::WC_SUCCESS) {
        update_message("Error in setting basedata");
    }

    // setting concurrency options
    if(_crawler.set_concurrency_options(
        ui->depthBox->value(),
        ui->threadBox->value()
    ) != webler::WC_SUCCESS) {
        update_message("Error in setting concurrency options");
    }

    // setting callback
    if(_crawler.set_callback(callback) != webler::WC_SUCCESS) {
        update_message("Error in setting callback");
    }

    // starting the crawler
    timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC,&start_time);
    _crawler.start();
    clock_gettime(CLOCK_MONOTONIC,&end_time);

    // collecting all failed URL
    ofstream outfile;
    string faildir = weblerdir + "/failed_url.txt";
    outfile.open(faildir.c_str());
    vector<webler::failed_url> fu = _crawler.get_failed_url();
    for(auto i: fu) {
        outfile << "Depth:" << i.depth << "\nURL:" << i.url << endl << endl;
    }

    outfile.close();

    crawler_started = false;
    ui->submit->setEnabled(true);

    cout << sessionid << " finished in " << end_time.tv_sec - start_time.tv_sec + (end_time.tv_nsec - start_time.tv_nsec)/1e09 << "seconds" << endl;
    update_message("Crawler finished. ID: " + sessionid);

}
