#ifndef PUTIN_H
#define PUTIN_H

#include <QMainWindow>

namespace Ui {
class putin;
}

class putin : public QMainWindow
{
    Q_OBJECT

public:
    explicit putin(QWidget *parent = 0);
    ~putin();

private slots:
    void on_pushButton_clicked();

    void on_typeurl_clicked();

private:
    Ui::putin *ui;
};

#endif // PUTIN_H
