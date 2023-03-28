#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QWidget>
#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class DBmanager;
}

class DBmanager : public QWidget
{
    Q_OBJECT

public:
    explicit DBmanager(QWidget *parent = 0);
    ~DBmanager();
private slots:
    void comboxChanged();
    void on_deleteButton_clicked();

    void on_insertButton_clicked();

    void on_searchButton_clicked();

private:
    Ui::DBmanager *ui;

    QSqlDatabase db;

    void settableWidget(QString str);//设置表格

    void clearAll();    //清空表格
};

#endif // DBMANAGER_H
