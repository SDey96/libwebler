#include "putin.h"
#include "ui_putin.h"
#include<QInputDialog>
#include<QString>

putin::putin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::putin)
{
    ui->setupUi(this);
}

putin::~putin()
{
    delete ui;
}

void putin::on_pushButton_clicked()
{
    int n=QInputDialog::getInt(this,tr("input"),tr("deapth"));
       ui->label->setText(QString::number(n));

}

void putin::on_typeurl_clicked()
{
    QString txt[5];
    for(int i=0; i<5; i++) {
        txt[i] = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                       tr("enter url:"), QLineEdit::Normal);
         ui->label_2->setText(txt[i]);
    }
//    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
//                                                tr("enter url:"), QLineEdit::Normal);
//        ui->label_2->setText(text);
}
