#include "mysocket.h"

MySocket::MySocket(QObject *parent) : QTcpSocket(parent)
{

}
MySocket::MySocket(qintptr hand)
{
    if(this->setSocketDescriptor(hand))
        qDebug()<<"new connection found";
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    preready = false;
    QString IP = peerAddress().toString().remove(0, 7);
    //receiveBlockSize = 0;
}

void MySocket::dataReceived()
{

   QByteArray buffer;
   if(preready == true)//是否接受完表头信息
   {
       if(this->bytesAvailable()<atoi(jpeg_size.data()))
           return;
       buffer = this->readAll();
       jpeg_data.append(buffer);
       //接受图像大小等于表头记录大小
       if(jpeg_data.size() >= atoi(jpeg_size.data()))
       {
           emit dataReady(IP,jpeg_data);
           return;
       }

   }
   //还未接受表头信息时
   if(preready == false)
   {

       buffer = this->readAll();
       if(QString(buffer) == "end")
       {
           emit dataReady(IP,jpeg_data);
           return;
       }
       if(jpeg_size.size()<5)
       {
           jpeg_size.append(buffer);
       }
       if(jpeg_size.size() == 5 ||jpeg_size.size() == 4)//是否接受长度为5的表头信息
       {
           preready = true;
           this->write("ok");
       }

   }


}
void MySocket::readyForNext()
{
    jpeg_data.clear();
    jpeg_size.clear();
    preready = false;
    this->write("ok");
}

void MySocket::readyForEnd()
{
    jpeg_data.clear();
    jpeg_size.clear();
    preready = false;
}
