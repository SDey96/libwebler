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
using namespace std;

int _depth = 0;
string _baseURL = "";
vector<string> _regexString;
bool crawler_started = false;
int message_count = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->messages->setReadOnly(true);
    current_message = "";
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {}

void MainWindow::on_lineEdit_textEdited(const QString &arg1) {
    _baseURL = arg1.toUtf8().constData();
}

void MainWindow::update_message(string msg) {
    current_message = string("[") + to_string(++message_count) + string("] ") + msg + string("\n") + current_message;
    ui->messages->document()->setPlainText(current_message.c_str());
}

void MainWindow::on_getDepth_clicked() {
    _depth = QInputDialog::getInt(this,tr("Enter Depth"),tr("Depth"));
    ui->depthValue->setText(QString::number(_depth));
    update_message(string("Depth set to ") + to_string(_depth));
    _regexString.clear();
    update_message("Regex vector cleared");
    string labels;
    for(int i=0; i<_depth; i++) {
        labels = string("Enter regex for depth ") + to_string(i+1) + string(":");
        _regexString.push_back( QInputDialog::getText(this, tr("Regex"),tr(labels.c_str()), QLineEdit::Normal).toUtf8().constData() );
        update_message(string("Regex ") + to_string(i+1) + string(" set to ") + _regexString.back());
    }

}

int counter = 0;
string sessionid;
string weblerdir;
string filedir;

void update_sessionid() {

    time_t now = time(0);
    tm *ltm = localtime(&now);

    sessionid = to_string(1900 + ltm->tm_year) + to_string(1 + ltm->tm_mon) + to_string(ltm->tm_mday) + to_string(1 + ltm->tm_hour) + to_string(1 + ltm->tm_min) + to_string(1 + ltm->tm_sec);
    weblerdir = "/home/codesome/webler/" + sessionid;
    QDir().mkdir(weblerdir.c_str());
}

void update_file_dir(int x) {
    filedir = weblerdir + "/result_" + to_string(x) + ".txt";
}

void callback(bool status, string url, vector<string> data) {

    counter++;

    ofstream outfile;
    update_file_dir(counter);
    outfile.open(filedir.c_str());

    if(status){
        outfile << "URL: " << url << endl;
        outfile << "Data: " << endl;;
        for(auto i: data) {
            outfile << i << endl << endl;
        }
        cout << filedir << endl;
    } else {
        outfile << "FAILED: " << endl;
        outfile << "URL: " << url << endl;
    }



    outfile.close();

}

void MainWindow::on_submit_clicked() {

    if(crawler_started){
        update_message("Crawling is in progress");
        return;
    }

    ui->submit->setEnabled(false);

    // update_message("Crawler started");
    crawler_started = true;
    update_sessionid();
    web::WebCrawler _crawler;

    // string regex1("<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks1\"[^>]*>([^?]*(?=</ul>))</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"unwantedLinks\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"wantedLinks2\"[^>]*>([^?]*(?=</ul>))</ul>");
    // string regex2("<\\s*ul\\s+[^>]*id\\s*=\\s*\"junkData\"[^>]*>[^?]*(?=</ul>)</ul>[^<]*<\\s*div\\s+[^>]*id\\s*=\\s*\"wantedData\"[^>]*>([^?]*(?=</div>))</div>[^<]*<\\s*ul\\s+[^>]*id\\s*=\\s*\"junkData2\"[^>]*>[^?]*(?=</ul>)</ul>");
    // vector<string> re = { regex1, regex1, regex1, regex1, regex1, regex1, regex1, regex1, regex1, regex1, regex2 };

    if(_crawler.set_basedata(
        _baseURL,
        _depth,
       _regexString
       // re
    ) != web::WC_SUCCESS) {
        update_message("Error in setting basedata");
    }

    if(_crawler.set_concurrency_options(3,5) != web::WC_SUCCESS) {
        update_message("Error in setting concurrency options");
    }

    if(_crawler.set_callback(callback) != web::WC_SUCCESS) {
        update_message("Error in setting callback");
    }

    _crawler.start();

    ofstream outfile;
    string faildir = weblerdir + "/failed_url.txt";
    outfile.open(faildir.c_str());

    vector<web::failed_url> fu = _crawler.get_failed_url();
    for(auto i: fu) {
        outfile << "Depth:" << i.depth << "\nURL:" << i.url << endl << endl;
    }

    outfile.close();

    crawler_started = false;
    ui->submit->setEnabled(true);

    update_message("Crawler finished");

}
