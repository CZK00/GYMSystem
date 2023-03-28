#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QMutexLocker>
#include "manager.h"
#include "mysocket.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QUdpSocket>
#include <myrunnable.h>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);

    MyThread(qintptr hand,int num);

    MyThread(QString dbName);

    QString getIP();

    void showManager();


private:    
    void run(void); //重写虚函数

    cv::Mat QImageToMat(QImage image);      //数据格式转化QImage->Mat

    QImage MatToQImage(const cv::Mat &mat); //数据格式转化Mat->QImage

    QImage QByteArrayToQImage(const QByteArray &ba);//数据格式转化QByteArray->QImage

    void detectFace(cv::Mat &frame);//人脸检测

    int Predict(cv::Mat src_image);  //识别图片

    void findMaxID(); //寻找最大次数的结果

    void database_Write(int opr);

    void turnOnDevice();

    QString database_Search(int face_num);   //数据库查询

    cv::CascadeClassifier face_cascade;

    vector<cv::Rect> faces;//用于保存检测结果的向量

    cv::Ptr<cv::face::FaceRecognizer> model;  //识别模型

    QHash<int,int> tempid;  //存储识别的序号

    bool managerIsShow;//子界面是否显示

    Manager *mymanager;//子界面指针

    MySocket *sock;

    QString IP;

    QTime time;

    QString name;

    int face_ID;

    int idnum;//识别的次数

    int thread_num;//线程号

    QList<QString> state_list;//该线程的状态信息


    static QMutex _mutex;//线程锁

    QSqlDatabase db;    //每个线程创建一个数据库连接

    QUdpSocket udp;



signals:
    void connection_Destroy(const QString &IP,const int &num);//连接端开返回信息
    void connection_Found(const QString &IP,const int &num);//新连接建立返回信息
    void speedFeedBack(const double &speed,const int &num);
public slots:

    void connection_Lose();

    void managerClose();

    void recData(const QString &IP, const QByteArray &data);

};

#endif // MYTHREAD_H
