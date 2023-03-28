#include "dbmanager.h"
#include "ui_dbmanager.h"

DBmanager::DBmanager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DBmanager)
{
    ui->setupUi(this);
    this->setWindowTitle("数据库管理");
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setHostName("127.0.0.1");
    //db.setUserName("root");
    //db.setPassword("123456");
    db.setDatabaseName("database.db");
    if(!db.open()) //如果数据库打开失败，会弹出一个警告窗口
    {
        QMessageBox::warning(this, "警告", "数据库打开失败");
    }
    //QString str = QString("insert into customer(face_ID,name,telNum,serviceType,surplusNum,deadline,enable) values('%1','%2','%3','%4','%5','%6','%7')").arg("2").arg("xiaohong").arg("13032313").arg("2").arg("0").arg("20210901").arg("1");
    //QString str = QString("insert into sportdata(face_ID,parent_ID,current_ID,data,time,place,heart_beats,temperature) values('%1','%2','%3','%4','%5','%6','%7','%8')").arg("1").arg("0").arg("1").arg("2021-03-26").arg("11:15:00").arg("1").arg("63").arg("37.5");

    //QSqlQuery query;
    //query.exec(str);

    /*
    else
    {
        qDebug()<<"数据库打开成功";


       QString str = "create table customer(face_ID int,name varchar(32),telNum varchar(32),serviceType int,surplusNum int,deadline varchar(32),enable int);";
        //QString str = "create table sportdata(face_ID int,parent_ID int,current_ID int,data DATE,time TIME,place int,heart_beats int,temperature float);";
        QSqlQuery query(db);
        bool success = query.exec(str);
        if(success)
        {
            qDebug()<<"创建表成功";
        }
        else
        {
            QSqlError lastError = query.lastError();
            qDebug() << lastError << QString(QObject::tr("插入失败"));
        }


    }
    */
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChanged()));
    settableWidget(ui->comboBox->currentText());

}

DBmanager::~DBmanager()
{
    delete ui;
}

void DBmanager::clearAll()
{
    for(int i = ui->tableWidget->rowCount(); i>=0 ;i--)
    {
        ui->tableWidget->removeRow(i);
    }
}

void DBmanager::comboxChanged()
{
    clearAll();
    settableWidget(ui->comboBox->currentText());

}

void DBmanager::settableWidget(QString str)
{
    if(str == "customer")
    {
        QStringList headers;
        headers << ("face_ID") << ("姓名") << ("电话") << ("套餐类型") << ("剩余次数") << ("截止日期") << ("是否可用");
        ui->tableWidget->setColumnCount(7);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0, 65);
        ui->tableWidget->setColumnWidth(1, 120);
        ui->tableWidget->setColumnWidth(2, 165);
        ui->tableWidget->setColumnWidth(3, 100);
        ui->tableWidget->setColumnWidth(4, 100);
        ui->tableWidget->setColumnWidth(5, 160);
        ui->tableWidget->setColumnWidth(6, 55);


        QString select_all_sql = "select * from customer";
        QSqlQuery sql_query;//QSqlQuery类提供执行和操作的SQL语句的方法

            //查询所有记录
            //sql_query.prepare(select_all_sql);
            if (!sql_query.exec(select_all_sql))
            {
                qDebug() << sql_query.lastError();//查询失败
            }
            else
            {
                //检索下一个
                while (sql_query.next())
                {
                    int ID = sql_query.value(0).toInt();
                    QString name = sql_query.value(1).toString();
                    QString telNum = sql_query.value(2).toString();
                    int serviceType = sql_query.value(3).toInt();
                    int surplusNum = sql_query.value(4).toInt();
                    QString deadline = sql_query.value(5).toString();
                    int enable = sql_query.value(6).toInt();

                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(ID)));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(name));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(telNum));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(QString::number(serviceType)));
                    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(QString::number(surplusNum)));
                    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(deadline));
                    ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem(QString::number(enable)));


                }
            }

        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    }
    else
    {
        QStringList headers;
        headers << ("face_ID") << ("parent_ID") << ("current_ID") << ("data") << ("time") << ("palce") << ("心率")<<("体温");
        ui->tableWidget->setColumnCount(8);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0, 65);
        ui->tableWidget->setColumnWidth(1, 100);
        ui->tableWidget->setColumnWidth(2, 100);
        ui->tableWidget->setColumnWidth(3, 100);
        ui->tableWidget->setColumnWidth(4, 100);
        ui->tableWidget->setColumnWidth(5, 100);
        ui->tableWidget->setColumnWidth(6, 100);
        ui->tableWidget->setColumnWidth(7, 100);
        QString select_all_sql = "select * from sportdata";
        QSqlQuery sql_query;//QSqlQuery类提供执行和操作的SQL语句的方法

            //查询所有记录
            //sql_query.prepare(select_all_sql);
            if (!sql_query.exec(select_all_sql))
            {
                qDebug() << sql_query.lastError();//查询失败
            }
            else
            {
                //检索下一个
                while (sql_query.next())
                {
                    int ID = sql_query.value(0).toInt();
                    int parent_ID = sql_query.value(1).toInt();
                    int current_ID = sql_query.value(2).toInt();
                    QString date = sql_query.value(3).toString();
                    QString time = sql_query.value(4).toString();
                    int place = sql_query.value(5).toInt();
                    int heart_beats = sql_query.value(6).toInt();
                    float temperature = sql_query.value(7).toFloat();

                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(ID)));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(QString::number(parent_ID)));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(QString::number(current_ID)));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(date));
                    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(time));
                    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(QString::number(place)));
                    ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem(QString::number(heart_beats)));
                    ui->tableWidget->setItem(rowCount,7,new QTableWidgetItem(QString("%1").arg(temperature)));

                }
            }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    }
}






void DBmanager::on_deleteButton_clicked()
{
    if(ui->comboBox->currentText() == "customer")
    {
    QSqlQuery query(db);
    int face_ID = ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().toInt();
    QString str = QString("delete from customer where face_ID = '%1'").arg(face_ID);
    query.exec(str);//删除信息
    clearAll();
    settableWidget("customer");
    }
    else
    {
        QSqlQuery query(db);
        int face_ID = ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text().toInt();
        int current_ID = ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text().toInt();
        QString str = QString("delete from sportdata where face_ID = '%1' and current_ID = '%2'").arg(face_ID).arg(current_ID);
        query.exec(str);//删除信息
        clearAll();
        settableWidget("sportdata");
    }
}

void DBmanager::on_insertButton_clicked()
{
    if(ui->insertButton->text() == tr("插入信息"))
    {
        clearAll();
        ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);//设置单元格不可编辑
        ui->tableWidget->insertRow(0);
        ui->insertButton->setText("确认插入");
    }
    else
    {
        if(ui->comboBox->currentText() == "customer")
        {
            int ID = ui->tableWidget->item(0,0)->text().toInt();
            QString name = ui->tableWidget->item(0,1)->text();
            QString telNum = ui->tableWidget->item(0,2)->text();
            int serviceType = ui->tableWidget->item(0,3)->text().toInt();
            int surplusNum = ui->tableWidget->item(0,4)->text().toInt();
            QString deadline = ui->tableWidget->item(0,5)->text();
            int enable = ui->tableWidget->item(0,6)->text().toInt();

            QString str = QString("insert into customer(face_ID,name,telNum,serviceType,surplusNum,deadline,enable) values('%1','%2','%3','%4','%5','%6','%7')").arg(ID).arg(name).arg(telNum).arg(serviceType).arg(surplusNum).arg(deadline).arg(enable);
            QSqlQuery query;
            query.exec(str);
            ui->tableWidget->removeRow(0);
            settableWidget("customer");
        }
        else
        {
            int ID = ui->tableWidget->item(0,0)->text().toInt();
            int parent_ID = ui->tableWidget->item(0,1)->text().toInt();
            int current_ID = ui->tableWidget->item(0,2)->text().toInt();
            QString date = ui->tableWidget->item(0,3)->text();
            QString time = ui->tableWidget->item(0,4)->text();
            int place = ui->tableWidget->item(0,5)->text().toInt();
            int heart_beats = ui->tableWidget->item(0,6)->text().toInt();
            float temperature = ui->tableWidget->item(0,7)->text().toFloat();
            QString str = QString("insert into sportdata(face_ID,parent_ID,current_ID,data,time,place,heart_beats,temperature) values('%1','%2','%3','%4','%5','%6','%7','%8')").arg(ID).arg(parent_ID).arg(current_ID).arg(date).arg(time).arg(place).arg(heart_beats).arg(temperature);
            QSqlQuery query;
            query.exec(str);
            ui->tableWidget->removeRow(0);
            settableWidget("sportdata");
        }
        ui->insertButton->setText("插入信息");
    }
}

void DBmanager::on_searchButton_clicked()
{
    if(ui->startTimeEdit->text() == NULL || ui->endTimeEdit->text() == NULL)
        {
        if(ui->nameEdit->text() == NULL)
            return;
        clearAll();
        QSqlQuery sql_query;
        QString name = ui->nameEdit->text();
        QString select_all_sql = QString("select * from customer WHERE name = '%1'").arg(name);
        sql_query.prepare(select_all_sql);
        if (!sql_query.exec())
        {
            qDebug() << sql_query.lastError();
        }
        else
        {
            while (sql_query.next())
            {
                int ID = sql_query.value(0).toInt();
                QString name = sql_query.value(1).toString();
                QString telNum = sql_query.value(2).toString();
                int serviceType = sql_query.value(3).toInt();
                int surplusNum = sql_query.value(4).toInt();
                QString deadline = sql_query.value(5).toString();
                int enable = sql_query.value(6).toInt();

                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowCount);
                ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(ID)));
                ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(name));
                ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(telNum));
                ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(QString::number(serviceType)));
                ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(QString::number(surplusNum)));
                ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(deadline));
                ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem(QString::number(enable)));


            }
        }

    }

    else if(ui->startTimeEdit->text() != NULL && ui->endTimeEdit->text() != NULL && ui->nameEdit->text() != NULL)
    {
    clearAll();
    QString startime = ui->startTimeEdit->text();
    QString endtime = ui->endTimeEdit->text();
    QString name  = ui->nameEdit->text();
    QSqlQuery sql_query;
    QString select_all_sql = "select * from sportdata WHERE time between:detectTime and:endTime and face_ID = :name";
    sql_query.prepare(select_all_sql);
    sql_query.bindValue(":detectTime", startime);
    sql_query.bindValue(":endTime", endtime);
    sql_query.bindValue(":name", name);
    if (!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        while (sql_query.next())
        {
            int ID = sql_query.value(0).toInt();
            int parent_ID = sql_query.value(1).toInt();
            int current_ID = sql_query.value(2).toInt();
            QString date = sql_query.value(3).toString();
            QString time = sql_query.value(4).toString();
            int place = sql_query.value(5).toInt();
            int heart_beats = sql_query.value(6).toInt();
            float temperature = sql_query.value(7).toFloat();

            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(rowCount);
            ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(ID)));
            ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(QString::number(parent_ID)));
            ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(QString::number(current_ID)));
            ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(date));
            ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(time));
            ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(QString::number(place)));
            ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem(QString::number(heart_beats)));
            ui->tableWidget->setItem(rowCount,7,new QTableWidgetItem(QString("%1").arg(temperature)));
            qDebug()<<rowCount;
    }
    }
    }

}
