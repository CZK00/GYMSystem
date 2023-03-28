#include "myrunnable.h"
extern QHash<QString,QList<QString>> DeviceList;

MyRunnable::MyRunnable(state da, QHostAddress add, quint16 p)
{
    datagram = da;
    address = add;
    port = p;
}
MyRunnable::~MyRunnable()
{

}

void MyRunnable::run()
{
    qDebug()<<"线程号："<<QThread::currentThreadId()<<"chipID:"<<datagram.chipid<<"face_ID:"<<datagram.face_ID<<"name:"<<datagram.heartbeats;
    QUdpSocket udp;
    QSqlDatabase db;
    //db = QSqlDatabase::database("database.db");
    if(QSqlDatabase::contains("default1"))
    {
        db = QSqlDatabase::database("default1");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE","default1");
        db.setDatabaseName("database.db");
        db.open();
    }
    QList<QString> temp;
    QSqlQuery query(db);
    QString str;
    int datacount = 1;
    if (datagram.na == NULL)
        return;
    switch (datagram.order) {
    case 0:
        if(DeviceList.contains(QString::number(datagram.chipid)))
        {
            DeviceList.remove(QString::number(datagram.chipid));
        }
        temp.append(address.toString().remove(0, 7));
        temp.append(QString::number(port));
        temp.append("未认证");
        DeviceList.insert(QString::number(datagram.chipid),temp);
        break;

    case 1:
        qDebug("this for order1");
        str = QString("insert into sportdata(face_ID,time,place,heart_beats,temperature) values('%1', '%2' ,'%3','%4','%5')").arg(QString::number(datagram.face_ID)).arg(QDateTime::currentDateTime().toString("yyyy/M/d hh:mm:ss")).arg(QString::number(datagram.place))
                .arg(QString::number(datagram.heartbeats)).arg(QString::number(datagram.temperature));
        query.exec(str);
        break;
    case 2:
        qDebug("this for order2");
        str = QString("select * from sportdata WHERE face_ID = '%1'").arg(datagram.face_ID);
        query.exec(str);
        while (query.next())
        {
            if(datagram.count == datacount)
                break;
            datacount++;
        }
        if(datagram.count == datacount)
        {
            datagram.place = query.value(4).toInt();
            datagram.heartbeats = query.value(5).toInt();
            datagram.temperature = query.value(6).toFloat();
        }
        udp.writeDatagram((char*)&datagram,sizeof(state),address,1122);
        break;
    case 3:
        qDebug("this for order2");
        str = QString("select * from customer WHERE face_ID = '%1'").arg(datagram.face_ID);
        query.exec(str);
        while (query.next())
        {
            datagram.surplusnum = query.value(4).toInt();
            strcpy(datagram.deadline,query.value(5).toString().toUtf8().data());
        }
        udp.writeDatagram((char*)&datagram,sizeof(state),address,1122);

        break;
    default:
        break;
    }
}
