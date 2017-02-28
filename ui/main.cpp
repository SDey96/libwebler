#include "putin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    putin w;
    w.show();

    return a.exec();
}
