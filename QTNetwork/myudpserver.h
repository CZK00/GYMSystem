#ifndef MYUDPERVER_H
#define MYUDPERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QThreadPool>
#include <myrunnable.h>

class MyUDPerver : public QObject
{
    Q_OBJECT

private:
    QUdpSocket *sock;//udp套接字

    QThreadPool *pool;//线程池

public:
    explicit MyUDPerver(QObject *parent = nullptr);

    void startService();//启动监听

signals:

public slots:
    void readData();//接收数据，分配到线程池处理数据

};

#endif // MYUDPERVER_H
