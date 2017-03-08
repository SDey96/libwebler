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
    /* Web Crawler */
    // When base URL is updated
    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_clicked();

    // When Enter Data button is clcked
    void on_getDepth_clicked();

    // When Start Crawling button is clicked
    void on_submit_clicked();
    
    // to update crawler logs
    void update_crawler_logs(string);
    
    /* Downloader */
    // When download URL is updated
    void on_lineEdit_2_textEdited(const QString &arg1);
    
    // When download file name is updated
    void on_lineEdit_3_textEdited(const QString &arg1);
    
    // When Start Downloading button is clicked
    void on_submit_2_clicked();

    // to update downloader logs
    void update_downloader_logs(string);

private:
    Ui::MainWindow *ui;

    // to store the logs
    string web_crawler_logs;
    string downloader_logs;

    // to store log counts
    int web_crawler_log_count;
    int downloader_log_count;

    // to store the base URL
    string _baseURL;
    
    // to store the depth
    int _depth;
    
    // to store all the regex
    vector<string> _regexString;

    // download file URL
    string _download_url;

    // downloaded file name
    string _download_file_name;
    

};

#endif // MAINWINDOW_H
