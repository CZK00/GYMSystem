#include "mythread.h"
extern QHash<QString,QList<QString>> ManagerList;
extern QHash<QString,QList<QString>> DeviceList;
QMutex MyThread::_mutex;



MyThread::MyThread(QObject *parent) : QThread(parent)
{
}
MyThread::MyThread(qintptr hand,int num)
{
    //_mutex.lock();
    if(!face_cascade.load("haarcascade_frontalface_alt2.xml"))
            qDebug()<<"error";

    mymanager = new Manager();
    mymanager->setWindowTitle("管理客户端"+IP);
    connect(mymanager,SIGNAL(shutDown()),this,SLOT(managerClose()));

    model = cv::face::FisherFaceRecognizer::create();//加载训练好的分类器
    model = cv::face::EigenFaceRecognizer::create();
    model->read("MyFacePCAModel.xml");

    db = QSqlDatabase::database("database.db");

    sock = new MySocket(hand);
    IP = sock->peerAddress().toString().remove(0,7);
    //sock->write("ok");

    idnum = 0;
    thread_num = num;
    state_list.append(IP);
    state_list.append("未认证");
    state_list.append("0bps");
    managerIsShow = false;
    //_mutex.unlock();
    //turnOnDevice();

}

MyThread::MyThread(QString dbName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setHostName("127.0.0.1");
    //db.setUserName("root");
    //db.setPassword("123456");
    db.setDatabaseName(dbName);
    if(!db.open()) //如果数据库打开失败，会弹出一个警告窗口
    {
        qDebug()<<"数据库打开失败";
    }
}


void MyThread::run()
{
    connect(sock,&MySocket::disconnected,this,&MyThread::connection_Lose);      //连接断开，释放线程
    connect(sock,SIGNAL(dataReady(QString,QByteArray)),this,SLOT(recData(QString,QByteArray)));
    //emit connection_Found(IP,thread_num);

    ManagerList.insert(QString("%1").arg(thread_num),state_list);
    //sock->write("start");
    time.start();
    this->exec();
}

void MyThread::connection_Lose()
{
    if(managerIsShow)
        delete mymanager;//删除创建的子界面
    sock->disconnectFromHost();
    //sock->abort();
    emit connection_Destroy(IP,thread_num);
    ManagerList.remove(QString("%1").arg(thread_num));     //更新界面列表
    model.~Ptr();
    face_cascade.~CascadeClassifier();

    this->quit();
    this->wait();
    sock->deleteLater();
    qDebug()<<"Disconnect";
}

QString MyThread::getIP()
{
    return IP;
}

void MyThread::showManager()
{
    if(!managerIsShow)
    {
    mymanager->show();
    managerIsShow = true;
    }

}

void MyThread::managerClose()
{
    managerIsShow = false;
}
void MyThread::recData(const QString &IP, const QByteArray &data)
{

    float usetime = time.elapsed();
    double speed = 8/usetime*1000/1024;
    state_list[2] = QString::number(speed,'f',2)+"kb";
    ManagerList.remove(QString("%1").arg(thread_num));
    ManagerList.insert(QString("%1").arg(thread_num),state_list);

    //emit speedFeedBack(speed,thread_num);
    if(idnum<15)
    {
        QImage img;
        img.loadFromData((const uchar *)data.data(),data.size());
        cv::Mat frame = QImageToMat(img);
        detectFace(frame);  //开启检测
        sock->readyForNext();
        time.start();
    }
    else
    {
        time.elapsed();
        findMaxID();
        name = database_Search(face_ID);
        qDebug()<<"识别结果："<<name<<face_ID;
        state_list[2] = "0bps";
        state_list[1] = name;
        ManagerList.insert(QString::number(thread_num),state_list);
        database_Write(1);
        idnum = 0;
        sock->readyForEnd();
        name = "未认证";
        face_ID = 0;
        //u.writeDatagram((char*)&datagram,sizeof(state),address,1122);
    }
    /*
    else if(idnum == 15)
    {
        time.elapsed();
        findMaxID();
        name = database_Search(face_ID);
        qDebug()<<"识别结果："<<name;
        state_list[2] = "0bps";
        state_list[1] = name;
        ManagerList.insert(QString::number(thread_num),state_list);
        database_Write(1);
        //写入face_ID
        char p[10];
        itoa(face_ID,p,10);
        sock->write(p);

        idnum++;
        sock->readyForEnd();

    }
    else
    {
        //写入name
        sock->write(name.toUtf8().data());
        idnum = 0;
    }
    */

    if(managerIsShow)
    {
        QImage img;
        img.loadFromData((const uchar *)data.data(),data.size());
        cv::Mat frame = QImageToMat(img);
        mymanager->showImage(MatToQImage(frame));
    }


}

cv::Mat MyThread::QImageToMat(QImage image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

QImage MyThread::MatToQImage(const cv::Mat &mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {

        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

QImage MyThread::QByteArrayToQImage(const QByteArray &ba)
{
    QString ss=QString::fromLatin1(ba.data(),ba.size());
    QByteArray rc;
    rc=QByteArray::fromBase64(ss.toLatin1());
    QByteArray rdc=qUncompress(rc);
    QImage img;
    img.loadFromData(rdc);
    return img;
}

QString MyThread::database_Search(int face_num)
{
    _mutex.lock();
    QString str = QString("select *from customer where face_ID = '%1'").arg(face_num);
    QSqlQuery query(db);
    query.exec(str);
    QString name;
    while(query.next())
    {
        name = query.value(1).toString();
    }
    _mutex.unlock();
    return name;
}

int MyThread::Predict(cv::Mat src_image)  //识别图片
{
    cv::Mat face_test;
    int predict = 0;
    //截取的ROI人脸尺寸调整
    if (src_image.rows >= 120)
    {
        //改变图像大小，使用双线性差值
        cv::resize(src_image, face_test, cv::Size(92, 112));

    }
    //判断是否正确检测ROI
    if (!face_test.empty())
    {
        //测试图像应该是灰度图
        predict = model->predict(face_test);
    }
    return predict;
}

void MyThread::detectFace(cv::Mat &frame)
{
    cv::Mat gray;
    cv::cvtColor(frame,gray,cv::COLOR_RGB2GRAY);
    cv::equalizeHist(gray,gray);

    face_cascade.detectMultiScale(gray,faces,1.1,2,0|cv::CASCADE_SCALE_IMAGE);
    if(faces.size() == 1)
    {
            cv::Point centera( faces[0].x, faces[0].y);
            cv::Point centerb( faces[0].x + faces[0].width, faces[0].y + faces[0].height );
            cv::rectangle(frame,centera,centerb,cv::Scalar(0,255,0),4);

            cv::Mat IDing = gray(faces[0]);
            if (IDing.empty())
                return;
            int faceID = Predict(IDing);
            if(tempid.contains(faceID))
            {
                tempid[faceID] = tempid.value(faceID) + 1;
            }
            else
            {
                tempid.insert(faceID,1);
            }
            QString name = database_Search(Predict(IDing));
            cv::putText(frame, QString("name:%1").arg(name).toStdString(), cv::Point(faces[0].x, faces[0].y), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255));//添加文字
            mymanager->addItem(name,MatToQImage(IDing));

    }
    idnum++;//采集的照片数自加1
}

void MyThread::findMaxID()
{
    int max = 0;
    QHash<int,int>::iterator j;
    for( j = tempid.begin();j != tempid.end();++j)
    {
        if(j.value() >= max)
        {
            max = j.value();
            face_ID = j.key();
        }
    }
    tempid.clear();
}

void MyThread::database_Write(int opr)
{
    QSqlQuery query(db);
    bool iscome = false;
    QString str = QString("insert into orderform(face_ID, name,time,operation,money,servicetype) values('%1', '%2' ,'%3','认证','0','0')").arg(face_ID).arg(name).arg(QDateTime::currentDateTime().toString("yyyy/M/d hh:mm:ss"));
    query.exec(str);
    QString str1 = QString("select * from orderform where face_ID = '%1' and operation = '认证'").arg(QString::number(face_ID));
    query.exec(str1);
    while(query.next())
    {
        QStringList temp;
        QStringList temp2;
        temp = query.value(2).toString().split("/");
        temp2 = QString(temp.at(2)).split(" ");
        if(QString(temp.at(1)).toInt() == QDate::currentDate().month() && QString(temp2.at(0)).toInt() == QDate::currentDate().day())
        {
            iscome = true;
            break;
        }
    }
    if(!iscome)
    {
        QString str2 = QString("select * from customer where face_ID = '%1'").arg(QString::number(face_ID));
        query.exec(str2);
        while(query.next())
        {
        int surplusNum = query.value(4).toInt();
        QString deadline = query.value(5).toString();
        int enable = query.value(6).toInt();
        QDateTime dl = QDateTime::fromString(deadline,"yyyy/M/d hh:mm:ss");
        if(enable == 1)
        {
            if(surplusNum == 0 && dl<QDateTime::currentDateTime())
                break;
            if(dl<QDateTime::currentDateTime())
            {
            surplusNum--;
            QString str3 = QString("update customer set surplusNum = '%1' where face_ID = '%2'").arg(QString::number(surplusNum)).arg(QString::number(face_ID));
            query.exec(str3);
            }
        }
        }
    }
    turnOnDevice();

}


void MyThread::turnOnDevice()
{
    QString IPtemp;//存放IP
    state temp;
    //寻找状态是未认证的设备
    for(int i = 0;i<DeviceList.count();i++)
    {
        if(DeviceList.values()[i][2] == tr("未认证"))
        {
            IPtemp = DeviceList.values()[i][0];
            QList<QString> x = DeviceList.values()[i];
            QString chipid = DeviceList.key(x);
            DeviceList.remove(chipid);
            x[2] = name;
            x.append(QString::number(face_ID));
            DeviceList.insert(chipid,x);
            break;
        }
    }
    //往数据包中填充认证的结果
    //temp.face_ID = face_ID;
    strcpy(temp.na,name.toUtf8().data());
    temp.face_ID = 409212823;
    //发送数据
    udp.writeDatagram((char*)&temp,sizeof(state),QHostAddress(IPtemp),1122);
}
