#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // When base URL is updated
    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_clicked();

    // When Enter Data button is clcked
    void on_getDepth_clicked();

    // When Start Crawling button is clicked
    void on_submit_clicked();


private:
    Ui::MainWindow *ui;

    // to store the logs of web crawler
    string web_crawler_logs;

    // to store the base URL
    string _baseURL;
    
    // to store the depth
    int _depth;
    
    // to store all the regex
    vector<string> _regexString;
    
    void update_message(string);
};

#endif // MAINWINDOW_H
