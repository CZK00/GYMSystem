#ifndef ENTRANCE_H
#define ENTRANCE_H

#include <QWidget>
#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include "widget.h"
#include <QString>

namespace Ui {
class entrance;
}

class entrance : public QWidget
{
    Q_OBJECT

public:
    explicit entrance(QWidget *parent = 0);

    ~entrance();

private slots:
    void on_logInButton_clicked();//登录

    void on_passwordChanged(QString temp);

    void changeAccount();//更改账户

private:
    Ui::entrance *ui;

    QSqlDatabase db;

    QString pass_word;


};

#endif // ENTRANCE_H
