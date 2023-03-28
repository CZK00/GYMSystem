#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QImageReader>

class webserver : public QObject
{
    Q_OBJECT
public:
    explicit webserver(QObject *parent = nullptr);    
private:
    QWebSocketServer *server;
    QSqlDatabase db;    //每个线程创建一个数据库连接
private slots:
    void onNewConnection();
    void msgComing(const QString &temp);
    void sendMsg();
};

#endif // WEBSERVER_H
