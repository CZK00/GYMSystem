#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QDataStream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/face.hpp"
#include<opencv2\face\facerec.hpp>
//using namespace cv;
using namespace std;
//using namespace cv::face;


class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(QObject *parent = nullptr);

    MySocket(qintptr hand);//析构函数，连接hand对应的套接字

    void readyForNext();

    void readyForEnd();

private:
    qint64 receiveBlockSize;//64bit数据传输的单位

    QByteArray jpeg_data;//jpeg数据

    QByteArray jpeg_size;//jpeg图像的大小

    QString IP;

    bool preready;

signals:

    void dataReady(const QString &IP, const QByteArray &data);//将socket层向上传给对应的线程

public slots:
    void dataReceived();//接收数据的槽函数

};

#endif // MYSOCKET_H
