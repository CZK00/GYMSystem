#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{

}
Server::Server(int port)
{
    listen(QHostAddress::Any,port);     //开启监听所有IP，端口8888
    thread_num = 0;
    //MyThread *thread = new MyThread("FaceDB.db");
}
void Server::incomingConnection(qintptr handle)
{
    MyThread *thread = new MyThread(handle,thread_num);
    //connect(thread,SIGNAL(connection_Found(QString,int)),this,SLOT(connectionFound(QString,int)));
    connect(thread,SIGNAL(connection_Destroy(QString,int)),this,SLOT(connectionDestroy(QString,int)));
    //connect(thread,SIGNAL(speedFeedBack(double,int)),this,SLOT(speedFeedBack(double,int)));

    list_thread.insert(thread_num,thread);//更新哈希表
    thread_num++;//更新进程号
    thread->start();
}

/*void Server::connectionFound(const QString &IP, const int &num)
{
    emit connection_Found(IP,num);
}*/
void Server::connectionDestroy(const QString &IP, const int &num)
{
    list_thread.remove(num);
    //emit connection_Destroy(IP,num);
}

int Server::selectThread(QString IP)
{
    for(int i = list_thread.size()-1;i>=0;i--)
    {
        if(IP == list_thread[i]->getIP())
        {
            return i;
        }
    }
}
void Server::showManager(const int num)
{
    list_thread.value(num)->showManager();
}
void Server::breakOFF(const int num)
{
    list_thread.value(num)->connection_Lose();
}
