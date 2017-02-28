#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <vector>
#include <regex>
#include<QInputDialog>
#include<QString>
#include <webler/WebCrawler.hpp>
using namespace std;

int _depth = 0;
string _baseURL = "";
vector<string> _regexString;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushButton_clicked() {

}


void MainWindow::on_lineEdit_textEdited(const QString &arg1) {
    _baseURL = arg1.toUtf8().constData();
}

void MainWindow::on_getDepth_clicked() {
    _depth = QInputDialog::getInt(this,tr("Enter Depth"),tr("Depth"));
    ui->depthValue->setText(QString::number(_depth));

    _regexString.clear();
    string labels;
    for(int i=0; i<_depth; i++) {
        labels = string("Enter regex for depth ") + to_string(i+1) + string(":");
        _regexString.push_back( QInputDialog::getText(this, tr("Regex"),tr(labels.c_str()), QLineEdit::Normal).toUtf8().constData() );
    }

}

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

void MainWindow::on_submit_clicked() {
    cout << _depth << endl;
    cout << _baseURL<< endl;

    for(auto i: _regexString) {
        cout << i << endl;
    }

    web::WebCrawler test_crawler;

        if(test_crawler.set_basedata(
            _baseURL,
            _depth,
            _regexString
        ) != web::WC_SUCCESS) {
            cout << "Error in setting metadata" << endl;

        }

        if(test_crawler.set_concurrency_options(3,5) != web::WC_SUCCESS) {
            cout << "Error in setting concurrency options" << endl;

        }

        if(test_crawler.set_callback(callback) != web::WC_SUCCESS) {
            cout << "Error in setting callback" << endl;

        }

        test_crawler.start();

        vector<web::failed_url> fu = test_crawler.get_failed_url();
        for(auto i: fu) {
            cout << "Failed:: Depth:" << i.depth << ", URL:" << i.url << endl;
        }
}
