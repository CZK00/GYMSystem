#include "myudpserver.h"

MyUDPerver::MyUDPerver(QObject *parent) : QObject(parent)
{

}
void MyUDPerver::startService()
{
    sock = new QUdpSocket(this);
    sock->bind(QHostAddress::Any,8888);//绑定socket和指定端口
    pool = new QThreadPool;
    pool->setMaxThreadCount(10);//设置最大线程数量
    connect(sock,SIGNAL(readyRead()),this,SLOT(readData()));
}

void MyUDPerver::readData()
{
    while(this->sock->hasPendingDatagrams())//判断是否有数据包流入
    {
        QHostAddress address;
        quint16 port;
        state datagram;
        sock->readDatagram((char*)&datagram,sizeof(state),&address,&port);//将数据包读入datagram中
        pool->start(new MyRunnable(datagram,address,port));//往线程池送入新的任务
    }
}
