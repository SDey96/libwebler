#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <vector>
#include <regex>
#include <QInputDialog>
#include <QString>
#include <QDir>
#include <webler/WebCrawler.hpp>
#include <webler/Downloader.hpp>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctime>
using namespace std;

Ui::MainWindow *global_ui;
// home directory of the system
string homedir = "";

// Main webler directory for web crawler
string weblerdir;

// Current web crawler ID (folder name to store data)
string sessionid;


// Window constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->messages->setReadOnly(true);
    global_ui = ui;
    
    web_crawler_logs = "";
    downloader_logs = "";
    web_crawler_log_count = 0;
    downloader_log_count = 0;
    _baseURL = "";
    _depth = 0;

    _download_url = "";
    _download_file_name = "";

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

/*==========================================================*/
/*=                     Web Crawler                        =*/
/*==========================================================*/

// true if the crawler has started
bool crawler_started = false;

// While in progress
// Number of callbacks from web crawler
int counter = 0;

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

// updating Base URL
void MainWindow::on_lineEdit_textEdited(const QString &arg1) {
    _baseURL = arg1.toUtf8().constData();
}

// to update the logs with new message
void MainWindow::update_crawler_logs(string msg) {
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
        update_crawler_logs("Depth set to " + to_string(_depth));
        _regexString.clear();
        update_crawler_logs("Regex vector cleared");

        // getting regexes
        for(int i=0; i<_depth; i++) {
            _regexString.push_back( QInputDialog::getText(this, 
                tr("Regex"),
                tr(("Enter regex for depth " + to_string(i+1) + "/" + to_string(_depth) + ":").c_str()), 
                QLineEdit::Normal).toUtf8().constData() );
            update_crawler_logs("Regex " + to_string(i+1) + " set to " + _regexString.back());
        }

    } else {
        // Invalid depth
        update_crawler_logs("Invalid depth " + to_string(_depth));
        _depth = 0;
        ui->depthValue->setText(QString::number(_depth));
    }

}

// to start the crawler
void MainWindow::on_submit_clicked() {

    counter = 0;

    if(crawler_started){
        update_crawler_logs("Crawling is in progress");
        return;
    }

    if(_baseURL == "") {
        update_crawler_logs("Error: Invalid base URL");
        return;
    }

    if(_depth <= 0) {
        update_crawler_logs("Error: Invalid depth");
        return;
    }

    if(_depth != _regexString.size()) {
        update_crawler_logs("Error: Invalid number of regex");
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
        update_crawler_logs("Error in setting basedata");
    }

    // setting concurrency options
    if(_crawler.set_concurrency_options(
        ui->depthBox->value(),
        ui->threadBox->value()
    ) != webler::WC_SUCCESS) {
        update_crawler_logs("Error in setting concurrency options");
    }

    // setting callback
    if(_crawler.set_callback(callback) != webler::WC_SUCCESS) {
        update_crawler_logs("Error in setting callback");
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
    update_crawler_logs("Crawler finished. ID: " + sessionid);

}




/*==========================================================*/
/*=                      Downloader                        =*/
/*==========================================================*/
// true if the downloader has started
bool downloader_started = false;

// to update the logs with new message
void MainWindow::update_downloader_logs(string msg) {
    // adding new msg to current msg
    downloader_logs = "[" + to_string(++downloader_log_count) + "] " + msg + "\n" + downloader_logs;
    // updating logs in UI
    ui->messages_2->document()->setPlainText(downloader_logs.c_str());
}

// updating download URL
void MainWindow::on_lineEdit_2_textEdited(const QString &arg1) {
    _download_url = arg1.toUtf8().constData();
}

// updating file name
void MainWindow::on_lineEdit_3_textEdited(const QString &arg1) {
    _download_file_name = arg1.toUtf8().constData();
}

void progressCallback(double percentage) {
    global_ui->progressBar->setValue((int)percentage);
}

// start downloading
void MainWindow::on_submit_2_clicked() {
    update_downloader_logs("Preparing to download.");
    if(_download_url == ""){
        update_downloader_logs("Terminating.");
        update_downloader_logs("Enter URL.");
        return;
    }
    if(downloader_started){
        update_downloader_logs("Already Downloading.");
        return;
    }
    
    downloader_started = true;
    global_ui->progressBar->setValue(0);
    
    ui->submit_2->setEnabled(false);
    
    update_downloader_logs("Downloading started.");
    webler::Downloader _downloader;
    _downloader.SetProgressCallback(progressCallback);
    if(_download_file_name == "") {
        if(_downloader.download(_download_url)) {
            update_downloader_logs("Downloading finished.");
        } else {
            update_downloader_logs("Downloading failed.");
        }
    } else {
        if(_downloader.download(_download_url,_download_file_name)){
            update_downloader_logs("Downloading finished.");
        } else {
            update_downloader_logs("Downloading failed.");
        }
    }

    downloader_started = false;

    ui->submit_2->setEnabled(true);
}