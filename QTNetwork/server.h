#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include "mythread.h"


class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    Server(int port);

    void incomingConnection(qintptr handle);        //重构，新tcp连接的建立

    void showManager(const int num);             //查看IP传输的数据

    void breakOFF(const int num);                //断开IP的连接

private:

     QHash<int,MyThread*> list_thread;//线程列表

    int selectThread(QString IP);//检索线程列表，选择对应IP的线程序号

    int thread_num;             //进程号，用于管理进程

signals:

    //void connection_Destroy(const QString &IP,const int &num);//连接端开返回信息

    //void connection_Found(const QString &IP,const int &num);//新连接建立返回信息

    //void netSpeedFeedBack(const double &speed,const int &num);

private slots:

   // void connectionFound(const QString &IP,const int &num);

    void connectionDestroy(const QString &IP,const int &num);

    //void speedFeedBack(const double &speed,const int &num);

};

#endif // SERVER_H
