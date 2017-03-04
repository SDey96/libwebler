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
    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_clicked();

    void on_getDepth_clicked();

    void on_submit_clicked();


private:
    Ui::MainWindow *ui;

    string current_message;
    
    void update_message(string);
};

#endif // MAINWINDOW_H
