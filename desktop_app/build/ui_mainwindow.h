/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *lineEdit;
    QLabel *baseHead;
    QLabel *depthHead;
    QPushButton *getDepth;
    QLabel *depthValue;
    QPushButton *submit;
    QLabel *label;
    QTextEdit *messages;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(621, 436);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(50, 90, 501, 31));
        baseHead = new QLabel(centralWidget);
        baseHead->setObjectName(QStringLiteral("baseHead"));
        baseHead->setGeometry(QRect(50, 70, 67, 17));
        depthHead = new QLabel(centralWidget);
        depthHead->setObjectName(QStringLiteral("depthHead"));
        depthHead->setGeometry(QRect(50, 140, 67, 17));
        getDepth = new QPushButton(centralWidget);
        getDepth->setObjectName(QStringLiteral("getDepth"));
        getDepth->setGeometry(QRect(140, 170, 89, 25));
        depthValue = new QLabel(centralWidget);
        depthValue->setObjectName(QStringLiteral("depthValue"));
        depthValue->setGeometry(QRect(50, 170, 71, 21));
        submit = new QPushButton(centralWidget);
        submit->setObjectName(QStringLiteral("submit"));
        submit->setGeometry(QRect(50, 350, 501, 31));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 20, 151, 31));
        messages = new QTextEdit(centralWidget);
        messages->setObjectName(QStringLiteral("messages"));
        messages->setGeometry(QRect(50, 210, 501, 131));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "webler", Q_NULLPTR));
        baseHead->setText(QApplication::translate("MainWindow", "Base URL", Q_NULLPTR));
        depthHead->setText(QApplication::translate("MainWindow", "Depth", Q_NULLPTR));
        getDepth->setText(QApplication::translate("MainWindow", "Enter Depth", Q_NULLPTR));
        depthValue->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        submit->setText(QApplication::translate("MainWindow", "Start Crawling", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:20pt; font-weight:600;\">webler</span></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
