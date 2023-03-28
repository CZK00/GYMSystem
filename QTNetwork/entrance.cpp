#include "entrance.h"
#include "ui_entrance.h"

entrance::entrance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::entrance)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    this->setWindowTitle("登录");
    //db.setHostName("127.0.0.1");
    //db.setUserName("root");
    //db.setPassword("123456");
    db.setDatabaseName("database.db");
    if(!db.open()) //如果数据库打开失败，会弹出一个警告窗口
    {
        QMessageBox::warning(this, "警告", "数据库打开失败");
    }

    connect(ui->password,SIGNAL(textChanged(QString)),this,SLOT(on_passwordChanged(QString)));
}



entrance::~entrance()
{
    delete ui;
}

void entrance::on_passwordChanged(QString temp)
{

    if(pass_word.size() > temp.size())
    {
        pass_word.chop(1);
    }
    else if(pass_word.size() < temp.size())
    {
    pass_word.append(temp.at(temp.size()-1));
    ui->password->setText(QString(temp.size(),'*'));
    }
}
void entrance::changeAccount()
{
    ui->user->clear();
    ui->password->clear();
    this->show();
}

void entrance::on_logInButton_clicked()
{
    QString userid = ui->user->text();
    //查询数据库administrators表中对应ID和password的数据
    QString temp=QString("select * from administrators where user_ID='"+userid+"' and password='"+pass_word+"'");
    QSqlQuery query(db);
    if(query.exec(temp)&&query.next())
    {
       //query.exec(s)是否执行成功,query.next()返回查询结果中的第一条
       QMessageBox::information(this, "提示", QString("登录成功,%1").arg(query.value("identification").toString()), QMessageBox::Yes, QMessageBox::Yes);
       Widget *mainwindow = new Widget(query.value("identification").toString(),0);//析构函数输入权限，根据权限分配不同的功能
       connect(mainwindow,SIGNAL(change_Account()),this,SLOT(changeAccount()));//注销时重现显示登录界面
       mainwindow->show();
       pass_word.clear();
       this->hide();
    }
    else
    {
       QMessageBox::warning(this,tr("Warning"),tr("用户名或密码错误"),QMessageBox::Ok);
    }
}
