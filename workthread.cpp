#include "workthread.h"

WorkThread::WorkThread(QObject *parent) : QThread(parent)
{
    ThreadTimer=new QTimer();
    connect(ThreadTimer,SIGNAL(timeout()),this,SLOT(Timeout()));
    ThreadTimer->moveToThread(this);
}

WorkThread::~WorkThread()
{
    ThreadTimer->stop();
    ThreadTimer->deleteLater();
}

void WorkThread::Timeout()
{
    if(!CompareDateTime.isEmpty() && !CompareDateTime.isNull())
    {
        QFileInfo FileInfo(FilePath);
        if(FileInfo.exists())
        {
            if(CompareDateTime!=FileInfo.lastModified().toString("yyyy-MM-dd HH:mm"))
            {
                CompareDateTime=FileInfo.lastModified().toString("yyyy-MM-dd HH:mm");
                emit MemoUpdate();
            }
        }
        else
        {
            CompareDateTime.clear();
        }
    }
    else
    {
        QFileInfo FileInfo(FilePath);
        if(FileInfo.exists())
        {
            CompareDateTime=FileInfo.lastModified().toString("yyyy-MM-dd HH:mm");
        }
    }
}

void WorkThread::run()
{
    QFileInfo FileInfo(FilePath);
    if(FileInfo.exists())
    {
        CompareDateTime=FileInfo.lastModified().toString("yyyy-MM-dd HH:mm");
    }

    ThreadTimer->start(5000);
    exec();
}
