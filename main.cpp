#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QSharedMemory shared("SharedMemo");
    if(!shared.create(512,QSharedMemory::ReadWrite))
    {
        QMessageBox::information(&w,QObject::tr("SharedMemo"),QObject::tr("Program excuting..."),QMessageBox::Ok);
        exit(0);
    }
    w.show();
    return a.exec();
}
