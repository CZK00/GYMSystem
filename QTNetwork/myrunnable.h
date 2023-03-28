#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QDebug>
#include <QThread>
#include <QUdpSocket>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlDatabase>
/*
 *order 0 - 设备开机连接
 *order 1 - 设备传输采集的数据
 *order 2 - 查询数据指令
 *order 3 - 查询身份指令
 *order 4 - 复位指令
*/
#pragma pack(1)
struct state{
    int order;          //指令
    int count;          //序号
    char na[10];        //姓名
    int face_ID;        //身份ID
    char deadline[20];  //套餐截至时间
    int surplusnum;     //套餐剩余次数
    int heartbeats;     //心率
    int place;          //位置
    float temperature;  //体温
    uint64_t chipid;    //设备编号
};
#pragma pack()



class MyRunnable : public QObject,public QRunnable
{
    Q_OBJECT
public:
    MyRunnable(state da,QHostAddress add,quint16 p);
    ~MyRunnable();

private:
    int count;  
    state datagram;//数据包
    QHostAddress address;
    quint16 port;


protected:
    void run();
signals:

public slots:
};

#endif // MYRUNNABLE_H
