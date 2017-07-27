#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QtWidgets>

class WorkThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkThread(QObject *parent = 0);
    ~WorkThread();

    QString FilePath;
signals:
    void MemoUpdate();
public slots:

private slots:
    void Timeout();

private:
    QString CompareDateTime;
    QTimer *ThreadTimer;

protected:
    void run();

};

#endif // WORKTHREAD_H
