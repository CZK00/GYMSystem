#include "webserver.h"

webserver::webserver(QObject *parent) : QObject(parent)
{

    if(QSqlDatabase::contains("default"))
    {
        db = QSqlDatabase::database("default");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE","default");
        db.setDatabaseName("database.db");
        db.open();
    }
    /*
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setHostName("127.0.0.1");
    //db.setUserName("root");
    //db.setPassword("123456");
    db.setDatabaseName("database.db");
    db.open();
    */
    server = new QWebSocketServer("Server",QWebSocketServer::NonSecureMode,this);
    server->listen(QHostAddress::Any,9997);
    connect(server,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
}
void webserver::onNewConnection()
{
    QWebSocket *sock = server->nextPendingConnection();
    connect(sock,&QWebSocket::textMessageReceived,[this,sock](const QString &msg){
            QStringList temp = msg.split("/");
            QString select_all_sql = QString("select * from sportdata WHERE face_ID = '%1'").arg(temp.at(0));
            QString select_all_sql2 =  QString("select * from customer WHERE face_ID = '%1'").arg(temp.at(0));
            QString select_all_sql3 =  QString("select * from orderform WHERE face_ID = '%1'").arg(temp.at(0));
            QSqlQuery sql_query;//QSqlQuery类提供执行和操作的SQL语句的方法

                //查询所有记录
                if (!sql_query.exec(select_all_sql))
                {
                    qDebug() << sql_query.lastError();//查询失败
                }
                else
                {
                    //检索下一个
                    while (sql_query.next())
                    {
                        QString datetime = sql_query.value(3).toString();
                        QString place = sql_query.value(4).toString();
                        QString heart_beats = sql_query.value(5).toString();
                        QString temperature = sql_query.value(6).toString();
                        sock->sendTextMessage("1\\"+datetime+"\\"+place+"\\"+heart_beats+"\\"+temperature);
                    }
                }
                sql_query.clear();
                //查询所有记录
                if (!sql_query.exec(select_all_sql2))
                {
                    qDebug() << sql_query.lastError();//查询失败
                }
                else
                {
                    //检索下一个
                    while (sql_query.next())
                    {
                        QString ID = sql_query.value(0).toString();
                        QString name = sql_query.value(1).toString();
                        QString telNum = sql_query.value(2).toString();
                        QString surplusNum = sql_query.value(4).toString();
                        QString deadline = sql_query.value(5).toString();
                        QString enable = sql_query.value(6).toString();
                        sock->sendTextMessage("2\\"+name+"\\"+ID+"\\"+telNum+"\\"+surplusNum+"\\"+deadline+"\\"+enable);
                    }
                }
                sql_query.clear();
                //查询所有记录
                if (!sql_query.exec(select_all_sql3))
                {
                    qDebug() << sql_query.lastError();//查询失败
                }
                else
                {
                    //检索下一个
                    while (sql_query.next())
                    {
                        QString time = sql_query.value(2).toString();
                        QString operation = sql_query.value(3).toString();
                        QString money = sql_query.value(4).toString();
                        QString servicetype = sql_query.value(5).toString();
                        sock->sendTextMessage("3\\"+time+"\\"+operation+"\\"+money+"\\"+servicetype);
                    }
                }
            sock->sendTextMessage("get successfully");
            });


    connect(sock,&QWebSocket::disconnected,[this,sock](){
            sock->deleteLater();
        });
    //connect(this,SIGNAL())

}
void webserver::msgComing(const QString &temp)
{
}
void webserver::sendMsg()
{

}

