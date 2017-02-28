/********************************************************************************
** Form generated from reading UI file 'putin.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUTIN_H
#define UI_PUTIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_putin
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLabel *label;
    QPushButton *typeurl;
    QLabel *label_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *putin)
    {
        if (putin->objectName().isEmpty())
            putin->setObjectName(QStringLiteral("putin"));
        putin->resize(810, 518);
        centralWidget = new QWidget(putin);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 20, 99, 27));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 80, 67, 17));
        typeurl = new QPushButton(centralWidget);
        typeurl->setObjectName(QStringLiteral("typeurl"));
        typeurl->setGeometry(QRect(10, 130, 481, 27));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(0, 190, 781, 61));
        putin->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(putin);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 810, 25));
        putin->setMenuBar(menuBar);
        mainToolBar = new QToolBar(putin);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        putin->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(putin);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        putin->setStatusBar(statusBar);

        retranslateUi(putin);

        QMetaObject::connectSlotsByName(putin);
    } // setupUi

    void retranslateUi(QMainWindow *putin)
    {
        putin->setWindowTitle(QApplication::translate("putin", "putin", 0));
        pushButton->setText(QApplication::translate("putin", "deapth", 0));
        label->setText(QString());
        typeurl->setText(QApplication::translate("putin", "typeurl", 0));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class putin: public Ui_putin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUTIN_H
