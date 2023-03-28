#include "widget.h"
#include "ui_widget.h"
QHash<QString,QList<QString>> ManagerList;     //全局变量
QHash<QString,QList<QString>> DeviceList;     //全局变量

Widget::Widget(QString na,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    identification = na;
    ui->label_welcome->setText(QString("欢迎你：")+identification);
    if(!face_cascade.load("E:\\opencv\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml"))
            qDebug()<<"error";

    heartchartInit();
    menuInit();
    severStart();
    menuSelect(0);

}


Widget::~Widget()
{
    delete ui;
}

void Widget::severStart()
{
    web = new webserver;
    imagecount = 0;
    collectstart = false;
    updateRate = new QTimer(this);
    timer = new QTimer(this);
    updateRate->start(100);
    connect(updateRate,SIGNAL(timeout()),this,SLOT(screenRefresh()));
    connect(timer,SIGNAL(timeout()),this,SLOT(readFrame()));
    myserver = new Server(8888);
    udpserver = new MyUDPerver(this);

    udpserver->startService();
    ui->IPManager->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //垂直滚动条
    ui->TcpState->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //垂直滚动条
    ui->ThreadNum->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //垂直滚动条
    connect(ui->Check,SIGNAL(clicked(bool)),this,SLOT(showClick()));
    connect(ui->NetSpeed->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(scrollBoxBind1()));
    connect(ui->IPManager->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(scrollBoxBind2()));
    connect(ui->TcpState->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(scrollBoxBind3()));
    connect(ui->ThreadNum->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(scrollBoxBind4()));
}

void Widget::menuInit()
{

    ui->dirbar->setMovement(QListView::Static);
    ui->dirbar->setViewMode(QListView::IconMode);
    ui->dirbar->addItem("<返回上一级>   ");
    ui->dirbar->addItem("您当前的位置:  > ");
    ui->dirbar->addItem("套餐查询 ");

    QSqlQuery query(db);
    QString str = QString("select *from combotype");
    query.exec(str);
    while(query.next())
    {
        ui->comboType->addItem(query.value("servicetype").toString());
    }

    ui->deviceEdit->setGeometry(260,130,200,20);
    ui->nameEdit2->setGeometry(520,130,200,20);
    ui->devicelabel->setGeometry(200,130,50,20);
    ui->namelabel->setGeometry(470,130,50,20);
    ui->deviceSearch->setGeometry(850,130,100,20);

    ui->orderbutton->setGeometry(580,480,60,20);

    ui->orderlabel1->setGeometry(200,160,50,20);
    ui->orderlabel2->setGeometry(200,190,50,20);
    ui->orderlabel3->setGeometry(200,220,50,20);
    ui->orderlabel4->setGeometry(200,250,50,20);

    ui->orderEdit1->setGeometry(260,160,200,20);
    ui->orderEdit2->setGeometry(260,190,200,20);
    ui->orderEdit3->setGeometry(260,220,200,20);
    ui->orderEdit4->setGeometry(260,250,400,200);

    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setIconSize(QSize(60,60));
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setSpacing(15);
    ui->listWidget->addItem(new QListWidgetItem(QIcon("系统维护.png"),"系统维护"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon("信息管理.png"),"信息管理"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon("数据查询.png"),"数据查询"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon("资源管理.png"),"资源管理"));
    ui->listWidget->addItem(new QListWidgetItem(QIcon("订单管理.png"),"订单管理"));
    //ui->listWidget->addItem(new QListWidgetItem(QIcon("xx.png"),"员工管理"));
    ui->listWidget->setFrameShape(QListWidget::NoFrame);
    ui->listWidget->setCurrentRow(0);


    ui->start_dateTime->setDate(QDate::currentDate());
    ui->end_dateTime->setDate(QDate::currentDate());
    ui->orderDate1->setDate(QDate::currentDate());
    ui->orderDate2->setDate(QDate::currentDate());
    ui->timesEdit->setEnabled(false);
    //菜单初始设定
    connect(ui->searchButton,SIGNAL(clicked(bool)),this,SLOT(searchSportData()));
    connect(ui->comboType,SIGNAL(currentIndexChanged(int)),this,SLOT(serviceTpyeChange()));
    ui->customerdatalist->setReadOnly(true);//QTestEdit
    ui->customerdatalist->setFont(QFont(tr("Consolas"), 10));
    ui->customerdatalist->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->routeEdit->setFont(QFont(tr("Consolas"), 10));
    ui->routeEdit->setTextInteractionFlags(Qt::NoTextInteraction);

    infield<<ui->label_14<<ui->label_13<<ui->label_12<<ui->label_11<<ui->nameEdit<<ui->telEdit<<ui->timesEdit<<ui->comboType<<ui->nextButton<<ui->label_25<<ui->serviceEdit;
    menu0<<ui->tableWidget<<ui->sysManager<<ui->newOrderButton<<ui->refreshOrderButton<<ui->deleteOrderButton;
    menu1<<ui->tableWidget<<ui->addNewCustom<<ui->refreshCustom;
    menu2<<ui->tableWidget<<ui->nameSearch<<ui->searchButton<<ui->label_8<<ui->faceidSearch<<ui->label_15;
    menu3<<ui->BreakOFF<<ui->Check<<ui->Count<<ui->IPManager<<ui->label<<ui->label_2<<ui->label_3<<ui->label_4
        <<ui->label_5<<ui->label_6<<ui->msgBox<<ui->NetSpeed<<ui->TcpState<<ui->ThreadNum<<ui->deviceMenu;
    menu4<<ui->tableWidget<<ui->orderAnalysis<<ui->orderList<<ui->orderSearch<<ui->ordershow<<ui->orderDate1<<ui->orderDate2<<ui->label_26<<ui->label_27;
    menu5<<ui->tableWidget<<ui->sysManager<<ui->newAccount<<ui->refreshAccount<<ui->deletAccount;
    menulast<<ui->searchWidget<<ui->ImageShow<<ui->customlabel<<ui->customlabel_3<<ui->customlabel_2<<ui->customlabel_4<<ui->customlabel_5<<ui->customlabel_6
           <<ui->label_9<<ui->label_10<<ui->start_dateTime<<ui->end_dateTime<<ui->showType<<ui->loginType<<ui->addmoneyCustom<<ui->fixCustom<<ui->freezeCustom<<ui->deleteCustom
          <<ui->drawwidget<<ui->label_17<<ui->label_18<<ui->label_19<<ui->label_21<<ui->label_22<<ui->label_23<<ui->label_20<<ui->label_16<<ui->placewidget<<ui->customerdatalist<<ui->label_24<<ui->routeButton
         <<ui->routeEdit<<ui->loginChart<<ui->loginEdit<<ui->accountChart<<ui->comboChart<<ui->ordershow2<<ui->ordershow3<<ui->label_28<<ui->label_29<<ui->nameSearch2<<ui->faceidSearch2<<ui->searchButton2<<ui->comboType2<<ui->label_30
        <<ui->orderEdit1<<ui->orderEdit2<<ui->orderEdit3<<ui->orderEdit4<<ui->orderlabel1<<ui->orderlabel2<<ui->orderlabel3<<ui->orderlabel4<<ui->orderbutton<<ui->nameEdit2<<ui->namelabel<<ui->deviceEdit<<ui->devicelabel<<ui->deviceSearch;
    menuorder<<ui->orderEdit1<<ui->orderEdit2<<ui->orderEdit3<<ui->orderEdit4<<ui->orderlabel1<<ui->orderlabel2<<ui->orderlabel3<<ui->orderlabel4<<ui->orderbutton;
   /* ui->menuWidget->setStyleSheet("QListWidget{color:rgb(173,175,178); background:rgb(25,27,31);border:0px solid gray;}"
                                    "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}"
                                    "QListWidget::Item:hover{color:rgb(255,255,255);background:transparent;border:0px solid gray;}"
                                    "QListWidget::Item:selected{border-image:url(images/listwidget_h.png); color:rgb(255,255,255);border:0px solid gray;}"
                                    "QListWidget::Item:selected:active{background:#00FFFFFF;color:#FFFFFF;border-width:0;}"
                                    );*/
    //ui->listWidget->setStyleSheet("QListWidget{color:rgb(0,0,0); background:rgb(200,200,200);border:0px solid gray;}");
    ui->ImageShow->setVisible(false);

    menuSelect(10);
    ui->tableWidget->setFrameShape(QListWidget::NoFrame);



    //设置
    connect(ui->listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(menuSet()));
    connect(ui->searchWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(searchDataType()));
    connect(ui->showType,SIGNAL(currentIndexChanged(int)),this,SLOT(changeShowType()));
    connect(ui->loginType,SIGNAL(currentIndexChanged(int)),this,SLOT(changeLoginType()));
    connect(ui->deviceMenu,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(deviceMenuSelect()));
    connect(ui->sysManager,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(systemManage()));
    connect(ui->orderDate1,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(changeOrderTime()));
    connect(ui->orderDate2,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(changeOrderTime()));
    connect(ui->dirbar,SIGNAL(clicked(QModelIndex)),this,SLOT(dirbarBack(QModelIndex)));
}




void Widget::heartchartInit()
{

    QPen penY(Qt::darkBlue,3,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin);
    QPen penY1(Qt::darkRed,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    heartchart = new QChart();
    heartseries = new QSplineSeries;
    teptseries = new QSplineSeries;
    timeseries = new QSplineSeries;
    axisYT = new QDateTimeAxis();
    axisY = new QValueAxis();
    axisY1 = new QValueAxis();
    axisX = new QValueAxis();
    drawtimer = new QTimer;
    routetimer = new QTimer;


    heartchart->setAnimationOptions(QChart::SeriesAnimations);
    heartchart->addSeries(heartseries);                            //把线添加到chart
    heartchart->addSeries(teptseries);

    heartchart->legend()->hide();                             //隐藏图例
    heartseries->setPointLabelsFormat("@yPoint");
    heartseries->setPointLabelsVisible(true);
    heartseries->setUseOpenGL(true);
    teptseries->setPointLabelsFormat("@yPoint");
    teptseries->setPointLabelsVisible(true);
    teptseries->setUseOpenGL(true);


    axisX->setTickCount(6);                             //设置坐标轴格数
    axisY->setTickCount(6);
    axisY1->setTickCount(6);
    axisYT->setTickCount(5);

    axisYT->setFormat("yyyy/M/d h:mm:ss");                        //设置时间显示格式
    axisY1->setLabelFormat("%.1f");

    axisY->setMin(0);                                    //设置Y轴范围
    axisY->setMax(120);
    axisY1->setMin(34);                                    //设置Y轴范围
    axisY1->setMax(40);

    axisX->setTitleText("时间");                       //设置X轴名称
    axisY->setLinePenColor(QColor(Qt::darkBlue));        //设置坐标轴颜色样式
    axisY->setGridLineColor(QColor(Qt::darkBlue));
    axisY1->setLinePenColor(QColor(Qt::darkRed));        //设置坐标轴颜色样式
    axisY1->setGridLineColor(QColor(Qt::darkRed));
    axisY->setGridLineVisible(false);                    //设置Y轴网格不显示
    axisY1->setGridLineVisible(false);                    //设置Y轴网格不显示

    axisY->setLinePen(penY);
    axisY1->setLinePen(penY1);
    axisX->setLinePen(penY);
    axisYT->setLinePen(penY);

    teptseries->setPen(penY1);
    heartseries->setPen(penY);

    heartchart->addAxis(axisX,Qt::AlignBottom);               //设置坐标轴位于chart中的位置
    heartchart->addAxis(axisY,Qt::AlignLeft);
    heartchart->addAxis(axisY1,Qt::AlignRight);
    heartchart->addAxis(axisYT,Qt::AlignLeft);


    timeseries->attachAxis(axisX);
    timeseries->attachAxis(axisYT);

    teptseries->attachAxis(axisX);
    teptseries->attachAxis(axisY1);

    heartseries->attachAxis(axisX);                           //把数据添加到坐标轴上
    heartseries->attachAxis(axisY);

    //heartchart->setAxisX(axisX1,heartseries);
    //heartchart->setAxisY(axisY,heartseries);

    axisY->setTitleText("心跳");
    axisY1->setTitleText("温度");
    axisYT->setVisible(false);
    //把chart显示到窗口上
    ui->drawwidget->setChart(heartchart);
    ui->drawwidget->setRenderHint(QPainter::Antialiasing);   //设置抗锯齿

    connect(drawtimer,SIGNAL(timeout()),this,SLOT(DrawLine()));

    //地图初始化
    routecount = 0;
    mapchart = new QChart();
    mapX = new QValueAxis();
    mapY = new QValueAxis();
    mapseries = new QSplineSeries();
    trackseries = new QLineSeries();
    mapoint = new QScatterSeries();
    mapoint2 = new QScatterSeries();

    mapchart->addSeries(trackseries);
    trackseries->setColor(QColor(255,255,255));

    mapchart->addSeries(mapseries);
    mapX->setTickCount(10);
    mapY->setTickCount(10);
    mapX->setRange(0,10);
    mapY->setRange(0,10);
    mapX->setVisible(false);
    mapY->setVisible(false);
    mapchart->addAxis(mapX,Qt::AlignBottom);               //设置坐标轴位于chart中的位置
    mapchart->addAxis(mapY,Qt::AlignLeft);
    mapchart->setAxisX(mapX,mapseries);
    mapchart->setAxisY(mapY,mapseries);
    mapchart->setAxisX(mapX,trackseries);
    mapchart->setAxisY(mapY,trackseries);
    mapseries->setColor(QColor(1,223,221));
    //*mapseries<< QPointF(1, 5)<< QPointF(2, 8)<< QPointF(3, 7)<< QPointF(5, 9)<< QPointF(6, 6)<< QPointF(7, 5)<< QPointF(6, 2)<< QPointF(4,3)<< QPointF(3, 1)<< QPointF(1, 5);

    mapoint->setMarkerShape(QScatterSeries::MarkerShapeCircle);//圆形的点
    mapoint->setBorderColor(QColor(255, 0, 0)); //离散点边框颜色
    mapoint->setBrush(QBrush(QColor(255, 0, 0)));//离散点背景色
    mapoint2->setMarkerShape(QScatterSeries::MarkerShapeCircle);//圆形的点
    mapoint2->setBorderColor(QColor(255, 255, 255)); //离散点边框颜色
    mapoint2->setBrush(QBrush(QColor(255, 255, 255)));//离散点背景色
    mapoint->setMarkerSize(15); //离散点大小
    mapoint2->setMarkerSize(8); //离散点大小
    *mapoint<< QPointF(1, 5)<< QPointF(2, 9)<< QPointF(5, 6)<< QPointF(6, 9)<< QPointF(9, 9)<< QPointF(8, 2)<< QPointF(5, 2)<< QPointF(4,3)<< QPointF(1, 2)<< QPointF(1, 5);
    *mapoint2<< QPointF(1, 5)<< QPointF(2, 9)<< QPointF(5, 6)<< QPointF(6, 9)<< QPointF(9, 9)<< QPointF(8, 2)<< QPointF(5, 2)<< QPointF(4,3)<< QPointF(1, 2)<< QPointF(1, 5);
    mapchart->addSeries(mapoint);
    mapoint->attachAxis(mapX);
    mapoint->attachAxis(mapY);
    mapchart->addSeries(mapoint2);
    mapoint2->attachAxis(mapX);
    mapoint2->attachAxis(mapY);
    //mapchart->setAxisX(mapX,mapoint);
    //mapchart->setAxisY(mapY,mapoint);
    mapchart->setTheme(QChart::ChartThemeDark);           //设置图标主题
    mapchart->setBackgroundVisible(false);
    //标签
    label = new QLabel(this);
    label->setStyleSheet(QString("QLabel{color:#1564FF; font-family:\"Microsoft Yahei\"; font-size:12px; font-weight:bold;"
                                 " background-color:rgba(21, 100, 255, 51); border-radius:4px; text-align:center;}"));
    label->setFixedSize(44, 24);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->hide();
    for(int i = 0;i<10;i++)
    {

        QLabel *mylabel = new QLabel(ui->placewidget);
        mylabel->setStyleSheet(QString("QLabel{color:#1564FF; font-family:\"Microsoft Yahei\"; font-size:12px; font-weight:bold;"
                                            " background-color:rgba(21, 100, 255, 51); border-radius:4px; text-align:center;}"));
        mylabel->setFixedSize(44, 24);
        mylabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        mylabel->setText(QString("place:%1").arg(QString::number(i)));
        mylabel->move(mapoint->at(i).x() - mylabel->width() / 2, mapoint->at(i).y() - mylabel->height() * 1.5);//移动数值
        mylabel->show();
        m_valueLabel.append(mylabel);
    }


    ui->placewidget->setRenderHints(QPainter::Antialiasing);//消除边缘，看起来平滑一些
    mapchart->setAnimationOptions(QChart::SeriesAnimations);

    ui->placewidget->setChart(mapchart);
    ui->placewidget->setStyleSheet(R"(QGraphicsView{ background-image:url(yy.png);})");


    connect(mapoint2,SIGNAL(hovered(QPointF,bool)),this,SLOT(deviceOnPoint(QPointF,bool)));
    connect(mapoint2,SIGNAL(clicked(QPointF)),this,SLOT(devicePlaceChoose(QPointF)));
    connect(routetimer,SIGNAL(timeout()),this,SLOT(DrawRoute()));


    //loginchart 初始化
    QChart *loginchart = new QChart();
    loginchart->setAnimationOptions(QChart::SeriesAnimations);
    loginseries = new QPieSeries;
    loginseries->setLabelsVisible(true);
    loginseries->setUseOpenGL(true);
    //loginseries->append("22",5);
    //loginseries->append("11",5);
    loginchart->setTheme(QChart::ChartThemeBrownSand);
    loginchart->addSeries(loginseries);
    ui->loginChart->setChart(loginchart);
    ui->loginChart->setRenderHint(QPainter::Antialiasing);   //设置抗锯齿


    QChart *accountchart = new QChart();
    accountchart->setAnimationOptions(QChart::SeriesAnimations);
    accountchart->addSeries(new QBarSeries());
    accountchart->addSeries(new QBarSeries());
    ui->accountChart->setChart(accountchart);
    ui->accountChart->chart()->createDefaultAxes();
    ui->accountChart->setRenderHint(QPainter::Antialiasing);   //设置抗锯齿

    QChart *combochart = new QChart();
    combochart->setAnimationOptions(QChart::SeriesAnimations);
    comboseries = new QPieSeries;
    comboseries->setLabelsVisible(true);
    comboseries->setUseOpenGL(true);
    combochart->setTheme(QChart::ChartThemeBlueCerulean);
    combochart->addSeries(comboseries);
    ui->comboChart->setChart(combochart);
    ui->comboChart->setRenderHint(QPainter::Antialiasing);   //设置抗锯齿


}

void Widget::dirbarBack(const QModelIndex &temp)
{
    ui->dirbar->clearSelection();
    qDebug()<<temp.row()<<temp.column();
    if(temp.row() == 0)
    {
        for(int i = 0;i<ui->listWidget->count();i++)
        {
            if(ui->listWidget->item(i)->text() == ui->dirbar->item(2)->text())
            {
                menuSelect(i);
                break;
            }
        }
    }
}

void Widget::systemManage()
{
    switch (ui->sysManager->currentRow()) {
    case 0:
        menuSelect(0);
        break;
    case 1:
        menuSelect(5);
    default:
        break;
    }
}

void Widget::showClick()
{
    if(ui->IPManager->selectedItems().size() == 1)
        myserver->showManager(ui->ThreadNum->item(ui->IPManager->currentRow())->text().toInt());
}

void Widget::on_BreakOFF_clicked()
{
    if(ui->IPManager->selectedItems().size() == 1)
        myserver->breakOFF(ui->ThreadNum->item(ui->IPManager->currentRow())->text().toInt());
}
void Widget::screenRefresh()
{
    if(ui->deviceMenu->currentRow() == 0)
    {
        for(int i = 0;i<ManagerList.size();i++)
        {
            if(ui->ThreadNum->findItems(ManagerList.keys()[i],Qt::MatchExactly).size() == 0)
            {
                ui->ThreadNum->addItem(ManagerList.keys()[i]);
                ui->IPManager->addItem(ManagerList.value(ManagerList.keys()[i])[0]);
                ui->TcpState->addItem(ManagerList.value(ManagerList.keys()[i])[1]);
                ui->NetSpeed->addItem(ManagerList.value(ManagerList.keys()[i])[2]);
                ui->ThreadNum->item(ui->ThreadNum->count()-1)->setFlags(Qt::ItemIsEnabled);
                ui->TcpState->item(ui->TcpState->count()-1)->setFlags(Qt::ItemIsEnabled);
                ui->TcpState->item(ui->TcpState->count()-1)->setTextColor(QColor(0,150,0,255));
                ui->NetSpeed->item(ui->NetSpeed->count()-1)->setFlags(Qt::ItemIsEnabled);
            }
        }

        for(int j = 0;j<ui->ThreadNum->count();j++)
        {
            if(!ManagerList.contains(ui->ThreadNum->item(j)->text()))
            {
                ui->IPManager->takeItem(j);
                ui->NetSpeed->takeItem(j);
                ui->TcpState->takeItem(j);
                ui->ThreadNum->takeItem(j);
            }
            else
            {
                ui->TcpState->item(j)->setText(ManagerList.value(ui->ThreadNum->item(j)->text())[1]);
                ui->NetSpeed->item(j)->setText(ManagerList.value(ui->ThreadNum->item(j)->text())[2]);
            }
        }
        ui->Count->setText(QString("%1").arg(ui->IPManager->count()));
    }
    else
    {
        for(int i = 0;i<DeviceList.size();i++)
        {
            if(ui->IPManager->findItems(DeviceList.keys()[i],Qt::MatchExactly).size() == 0)
            {
                ui->IPManager->addItem(DeviceList.keys()[i]);
                ui->ThreadNum->addItem(DeviceList.value(DeviceList.keys()[i])[0]);
                ui->TcpState->addItem(DeviceList.value(DeviceList.keys()[i])[1]);
                ui->NetSpeed->addItem(DeviceList.value(DeviceList.keys()[i])[2]);
                ui->ThreadNum->item(ui->ThreadNum->count()-1)->setFlags(Qt::ItemIsEnabled);
                ui->TcpState->item(ui->TcpState->count()-1)->setFlags(Qt::ItemIsEnabled);
                ui->NetSpeed->item(ui->TcpState->count()-1)->setTextColor(QColor(0,150,0,255));
                ui->NetSpeed->item(ui->NetSpeed->count()-1)->setFlags(Qt::ItemIsEnabled);
            }
        }

        for(int j = 0;j<ui->ThreadNum->count();j++)
        {
            if(!DeviceList.contains(ui->IPManager->item(j)->text()))
            {
                ui->IPManager->takeItem(j);
                ui->NetSpeed->takeItem(j);
                ui->TcpState->takeItem(j);
                ui->ThreadNum->takeItem(j);
            }
            else
            {
                ui->NetSpeed->item(j)->setText(DeviceList.value(ui->IPManager->item(j)->text())[2]);
            }
        }
        ui->Count->setText(QString("%1").arg(ui->IPManager->count()));
    }
}


void Widget::scrollBoxBind1()
{
    ui->IPManager->verticalScrollBar()->setValue(ui->NetSpeed->verticalScrollBar()->value());
    ui->TcpState->verticalScrollBar()->setValue(ui->NetSpeed->verticalScrollBar()->value());
    ui->ThreadNum->verticalScrollBar()->setValue(ui->NetSpeed->verticalScrollBar()->value());
}

void Widget::scrollBoxBind2()
{
    ui->NetSpeed->verticalScrollBar()->setValue(ui->IPManager->verticalScrollBar()->value());
    ui->TcpState->verticalScrollBar()->setValue(ui->IPManager->verticalScrollBar()->value());
    ui->ThreadNum->verticalScrollBar()->setValue(ui->NetSpeed->verticalScrollBar()->value());
}

void Widget::scrollBoxBind3()
{
    ui->IPManager->verticalScrollBar()->setValue(ui->TcpState->verticalScrollBar()->value());
    ui->NetSpeed->verticalScrollBar()->setValue(ui->TcpState->verticalScrollBar()->value());
    ui->ThreadNum->verticalScrollBar()->setValue(ui->NetSpeed->verticalScrollBar()->value());
}
void Widget::scrollBoxBind4()
{
    ui->IPManager->verticalScrollBar()->setValue(ui->ThreadNum->verticalScrollBar()->value());
    ui->NetSpeed->verticalScrollBar()->setValue(ui->ThreadNum->verticalScrollBar()->value());
    ui->TcpState->verticalScrollBar()->setValue(ui->ThreadNum->verticalScrollBar()->value());
}
/*
void Widget::on_db_Button_clicked()
{
    QString password = QInputDialog::getText(NULL, "Administrator",
                                                       "请输入密码",
                                                       QLineEdit::Normal,
                                                       "",
                                                       NULL);
    if(password == "123456")
    {
       QMessageBox::information(this, "Title", "登录成功", QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
       QMessageBox::warning(this,tr("Warning"),tr("用户名或密码错误"),QMessageBox::Ok);
    }
}*/
void Widget::menuSet()
{
    menuSelect(ui->listWidget->currentRow());
}

void Widget::menuSelect(int num)//选择菜单
{
    setInfiedVisable(false);
    ui->dirbar->item(2)->setText(QString("%1").arg(ui->listWidget->currentItem()->text()));
    if(ui->dirbar->count()>3)
    {
        for(int i = ui->dirbar->count()-1;i>=3;i--)
        {
            ui->dirbar->takeItem(i);
        }
    }
    switch (num) {
    case 0:
        if(identification == "员工")
        {
            QMessageBox::warning(this,tr("Warning"),tr("权限不足"),QMessageBox::Ok);
            break;
        }
        setMenu2Visable(false);
        setMenu3Visable(false);
        setMenu4Visable(false);
        setMenu1Visable(false);
        setMenu5Visable(false);
        setMenulastVisable(false);
        setMenu0Visable(true);
        break;
    case 1:
        setMenu0Visable(false);
        setMenu2Visable(false);
        setMenu3Visable(false);
        setMenu4Visable(false);
        setMenu5Visable(false);
        setMenulastVisable(false);
        setMenu1Visable(true);
        break;
    case 2:
        setMenu0Visable(false);
        setMenu1Visable(false);
        setMenu3Visable(false);
        setMenu4Visable(false);
        setMenu5Visable(false);
        setMenulastVisable(false);
        setMenu2Visable(true);
        break;
    case 3:
        setMenu0Visable(false);
        setMenu2Visable(false);
        setMenu1Visable(false);
        setMenu4Visable(false);
        setMenu5Visable(false);
        setMenulastVisable(false);
        setMenu3Visable(true);
        break;
    case 4:
        setMenu0Visable(false);
        setMenu2Visable(false);
        setMenu1Visable(false);
        setMenu3Visable(false);
        setMenu5Visable(false);
        setMenulastVisable(false);
        setMenu4Visable(true);
        break;
    case 5:
        setMenu0Visable(false);
        setMenu2Visable(false);
        setMenu1Visable(false);
        setMenu3Visable(false);
        setMenu4Visable(false);
        setMenulastVisable(false);
        setMenu5Visable(true);
        break;
    default:
        setMenu4Visable(false);
        setMenu3Visable(false);
        setMenu2Visable(false);
        setMenu1Visable(false);
        setMenu0Visable(false);
        setMenu5Visable(false);
        setInfiedVisable(false);
        setMenulastVisable(false);
    }
}

void Widget::setInfiedVisable(bool istrue)
{
    ui->telEdit->clear();
    ui->timesEdit->clear();
    ui->nameEdit->clear();
    for(int i = 0; i < infield.count(); i++)
                infield[i]->setVisible(istrue);
    QString select_all_sql = "select * from combotype";
    QSqlQuery sql_query;//QSqlQuery类提供执行和操作的SQL语句的方法
    if (!sql_query.exec(select_all_sql))
    {
        qDebug() << sql_query.lastError();//查询失败
    }
    else
    {
        ui->serviceEdit->clear();
        //检索下一个
        while (sql_query.next())
        {
            ui->serviceEdit->append(QString("套餐序号:%1 金额:%2 说明:%3").arg(sql_query.value(0).toString()).arg(sql_query.value(1).toString()).arg(sql_query.value(4).toString()));
            ui->serviceEdit->append("");
        }
    }



}

void Widget::setMenu0Visable(bool istrue)
{
    for(int i = 0; i < menu0.count(); i++)
                menu0[i]->setVisible(istrue);
    if(istrue == true)
    {
        dbList("combotype");
    }
}

void Widget::setMenu1Visable(bool istrue)
{
    for(int i = 0; i < menu1.count(); i++)
                menu1[i]->setVisible(istrue);
    if(istrue == true)
    {
        dbList("customer");
    }
}

void Widget::setMenu2Visable(bool istrue)
{
    for(int i = 0; i < menu2.count(); i++)
                menu2[i]->setVisible(istrue);
    if(istrue == true)
    {
        dbList("sportdata");
    }
}

void Widget::setMenu3Visable(bool istrue)
{
    for(int i = 0; i < menu3.count(); i++)
                menu3[i]->setVisible(istrue);

}

void Widget::setMenu4Visable(bool istrue)
{
    for(int i = 0; i < menu4.count(); i++)
                menu4[i]->setVisible(istrue);
    if(istrue == true)
    {
        dbList("orderform");
    }
}

void Widget::setMenu5Visable(bool istrue)
{
    for(int i = 0; i < menu5.count(); i++)
                menu5[i]->setVisible(istrue);
    if(istrue == true)
    {
        dbList("administrators");
    }
}
void Widget::setMenulastVisable(bool istrue)
{
    for(int i = 0; i < menulast.count(); i++)
                menulast[i]->setVisible(istrue);
}


void Widget::tableClearAll()//清除表单
{
    for(int i = ui->tableWidget->rowCount(); i>=0 ;i--)
    {
        ui->tableWidget->removeRow(i);
    }
}

void Widget::dbList(QString str)
{
    tableClearAll();
    ui->tableWidget->setGeometry(190,160,911,600);
    if(str == "customer")
    {
        QStringList headers;
        headers << ("face_ID") << ("姓名") << ("电话") << ("套餐类型") << ("剩余次数") << ("截止日期") << ("是否可用")<<("操作")<<("");
        ui->tableWidget->setColumnCount(9);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0, 125);
        ui->tableWidget->setColumnWidth(1, 120);
        ui->tableWidget->setColumnWidth(2, 135);
        ui->tableWidget->setColumnWidth(3, 100);
        ui->tableWidget->setColumnWidth(4, 100);
        ui->tableWidget->setColumnWidth(5, 150);
        ui->tableWidget->setColumnWidth(6, 55);
        ui->tableWidget->setColumnWidth(7,80);
        ui->tableWidget->setColumnWidth(8,80);
        ui->tableWidget->setFixedWidth(960);



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
    else if(str == "sportdata")
    {
        ui->tableWidget->setGeometry(0,160,911,600);
        QStringList headers;
        headers << ("face_ID") << ("parent_ID") << ("current_ID") << ("time") << ("palce") << ("心率")<<("体温");
        ui->tableWidget->setColumnCount(7);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0,125);
        ui->tableWidget->setColumnWidth(1, 100);
        ui->tableWidget->setColumnWidth(2, 100);
        ui->tableWidget->setColumnWidth(3, 150);
        ui->tableWidget->setColumnWidth(4, 100);
        ui->tableWidget->setColumnWidth(5, 100);
        ui->tableWidget->setColumnWidth(6, 100);
        ui->tableWidget->setFixedWidth(810);
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
                    QString datetime = sql_query.value(3).toString();
                    int place = sql_query.value(4).toInt();
                    int heart_beats = sql_query.value(5).toInt();
                    float temperature = sql_query.value(6).toFloat();

                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(ID)));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(QString::number(parent_ID)));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(QString::number(current_ID)));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(datetime));
                    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(QString::number(place)));
                    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(QString::number(heart_beats)));
                    ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem(QString("%1").arg(temperature)));

                }
            }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑

    }
    else if(str == "orderform")
    {
        int accounts = 0;
        int counts = 0;
        ui->tableWidget->setGeometry(0,160,811,600);
        QStringList headers;
        headers << ("face_ID") << ("name") << ("time") << ("operation") << ("money") << ("servicetype");
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0, 125);
        ui->tableWidget->setColumnWidth(1, 100);
        ui->tableWidget->setColumnWidth(2, 230);
        ui->tableWidget->setColumnWidth(3, 150);
        ui->tableWidget->setColumnWidth(4, 100);
        ui->tableWidget->setColumnWidth(5, 100);
        ui->tableWidget->setFixedWidth(810);
        QString select_all_sql = "select * from orderform";
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
                    int ID = sql_query.value("face_ID").toInt();
                    QString name = sql_query.value("name").toString();
                    QString time = sql_query.value("time").toString();
                    QString operation = sql_query.value("operation").toString();
                    int money = sql_query.value("money").toInt();
                    int servivetype = sql_query.value("servicetype").toInt();

                    if(operation == tr("注册"))
                    {
                        accounts += money;
                    }
                    else
                    {
                        counts++;
                    }

                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(ID)));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(name));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(time));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(operation));
                    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(QString::number(money)));
                    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(QString::number(servivetype)));
                }
            }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
        ui->ordershow->clear();
        ui->ordershow->append(QString("销售总额：%1(元)").arg(QString::number(accounts)));
        ui->ordershow->append("");
        ui->ordershow->append(QString("来访人数：%1").arg(QString::number(counts)));
    }

    else if(str == "combotype")
    {
        QStringList headers;
        headers << ("servicetype") << ("money") << ("time") << ("times") << ("supply");
        ui->tableWidget->setColumnCount(5);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0, 100);
        ui->tableWidget->setColumnWidth(1, 100);
        ui->tableWidget->setColumnWidth(2, 230);
        ui->tableWidget->setColumnWidth(3, 150);
        ui->tableWidget->setColumnWidth(4, 400);
        ui->tableWidget->setFixedWidth(1000);
        QString select_all_sql = "select * from combotype";
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
                    int servicetype = sql_query.value("servicetype").toInt();
                    QString money = sql_query.value("money").toString();
                    QString time = sql_query.value("time").toString();
                    QString times = sql_query.value("times").toString();
                    QString supply = sql_query.value("supply").toString();

                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(servicetype)));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(money));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(time));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(times));
                    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(supply));
                }
            }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    }

    else if(str == "administrators")
    {

        QStringList headers;
        headers << ("identification") << ("user_ID") << ("password") << ("name") << ("telNum")<<("lastime");
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0, 100);
        ui->tableWidget->setColumnWidth(1, 100);
        ui->tableWidget->setColumnWidth(2, 230);
        ui->tableWidget->setColumnWidth(3, 150);
        ui->tableWidget->setColumnWidth(4, 100);
        ui->tableWidget->setColumnWidth(5, 150);
        ui->tableWidget->setFixedWidth(850);
        QString select_all_sql = "select * from administrators";
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
                    QString identification = sql_query.value("identification").toString();
                    QString user_ID = sql_query.value("user_ID").toString();
                    QString password = sql_query.value("password").toString();
                    QString name = sql_query.value("name").toString();
                    QString telNum = sql_query.value("telNum").toString();
                    QString lastime = sql_query.value("lastime").toString();


                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(identification));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(user_ID));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(password));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(name));
                    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(telNum));
                    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(lastime));
                }
            }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    }
}

void Widget::changeOrderTime()
{
    int accounts = 0;
    int counts = 0;
    tableClearAll();
    QStringList headers;
    headers << ("face_ID") << ("name") << ("time") << ("operation") << ("money") << ("servicetype");
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->setColumnWidth(0, 125);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 230);
    ui->tableWidget->setColumnWidth(3, 150);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(5, 100);
    ui->tableWidget->setFixedWidth(810);
    QString select_all_sql;

    if(!ui->nameSearch2->isVisible())
    {
        select_all_sql =  "select * from orderform";
    }
    else
    {
        if(ui->comboType2->currentText() == tr("全部"))
        {
            select_all_sql = QString("select * from orderform where face_ID = '%1'").arg(face_ID);
        }
        else
        {
            select_all_sql = QString("select * from orderform where face_ID = '%1' and operation = '%2'").arg(face_ID).arg(ui->comboType2->currentText());
        }
    }
    QSqlQuery sql_query;//QSqlQuery类提供执行和操作的SQL语句的方法
    QDateTime startime = ui->orderDate1->dateTime();
    QDateTime endtime = ui->orderDate2->dateTime();

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
                int ID = sql_query.value("face_ID").toInt();
                QString name = sql_query.value("name").toString();
                QString time = sql_query.value("time").toString();
                QDateTime qtime = QDateTime::fromString(time,"yyyy/M/d hh:mm:ss");
                QString operation = sql_query.value("operation").toString();
                int money = sql_query.value("money").toInt();
                int servivetype = sql_query.value("servicetype").toInt();

                if(qtime < endtime && qtime > startime)
                {
                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowCount);
                ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(ID)));
                ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(name));
                ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(time));
                ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(operation));
                ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(QString::number(money)));
                ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(QString::number(servivetype)));
                if(operation == tr("注册"))
                {
                    accounts += money;
                }
                else
                {
                    counts++;
                }
                }
            }
        }
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    ui->ordershow->clear();
    ui->ordershow->append(QString("销售总额：%1（元）").arg(QString::number(accounts)));
    ui->ordershow->append("");
    ui->ordershow->append(QString("来访人数：%1").arg(QString::number(counts)));
    if(ui->accountChart->isVisible())
    {
       on_orderAnalysis_clicked();
    }
}


void Widget::on_pushButton_clicked()
{
    emit change_Account();
    delete this;
    delete ui;
}

void Widget::deleteCustomerData()
{
    int num = 4;
    QString faceID = ui->tableWidget->itemAt(num,4)->text();
    qDebug()<<faceID;
    //QSqlQuery query(db);
    QString str = QString("delete from customer where face_ID = '%1'").arg(faceID);
    //query.exec(str);//删除信息
    tableClearAll();
    dbList("customer");
}

void Widget::refreshCustomerData()
{

}


void Widget::serviceTpyeChange()
{
    if(ui->comboType->currentText() == "3")
        ui->timesEdit->setEnabled(true);
    else
        ui->timesEdit->setEnabled(false);
}

void Widget::on_addNewCustom_clicked()
{
    menuSelect(10);
    setInfiedVisable(true);
    ui->comboType->clear();
    QSqlQuery query(db);
    QString str = QString("select *from combotype");
    query.exec(str);
    while(query.next())
    {
        ui->comboType->addItem(query.value("servicetype").toString());
    }
}

void Widget::on_nextButton_clicked()
{

    if(ui->nextButton->text() == tr("下一步"))
    {
    if(ui->nameEdit->text() == NULL || ui->telEdit->text() == NULL)
    {
        QMessageBox::warning(this,tr("Warning"),tr("请输入完整信息"),QMessageBox::Ok);
        return;
    }
    if(ui->comboType->currentText() == "3" && ui->timesEdit->text() == NULL)
    {
        QMessageBox::warning(this,tr("Warning"),tr("请输入完整信息"),QMessageBox::Ok);
        return;
    }
    face_ID = QDateTime::currentDateTime().toString("MMddhhmmss").toInt();
    name = ui->nameEdit->text();
    telnum = ui->telEdit->text();

    addCustomerDB();

    ui->nextButton->setText("开始采集");
    setInfiedVisable(false);
    ui->ImageShow->setVisible(true);
    ui->nextButton->setVisible(true);
    cap.open(0);
    timer->start(50);

    }
    else
    {
        collectstart =true;
    }
}

void Widget::readFrame()
{
    cap.read(src_image);
    detectFace(src_image);
    QImage imag = MatImageToQt(src_image);
    ui->ImageShow->setPixmap(QPixmap::fromImage(imag));
    ui->ImageShow->resize(imag.size());
}

void Widget::detectFace(cv::Mat &frame)
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

            if(collectstart)
            {
                QDir folder;
                if(folder.exists(QString("%1/faceDB/%2").arg(QDir::currentPath()).arg(QString::number(face_ID))))
                {
                    if(imagecount<=15)
                    {
                    cv::Mat dst;
                    cv::resize(src_image(faces[0]), dst, cv::Size(92, 112));
                    cv::cvtColor(dst,dst,cv::COLOR_RGB2GRAY);
                    cv::imwrite(QString("%1/faceDB/%2/%3%4").arg(QDir::currentPath()).arg(QString::number(face_ID)).arg(QString::number(imagecount)).arg(".jpg").toStdString(), dst);
                    write_FaceDir(QDir::currentPath(),QString::number(face_ID),imagecount);
                    qDebug()<<QString("%1/faceDB/%2/%3%4").arg(QDir::currentPath()).arg(QString::number(face_ID)).arg(QString::number(imagecount)).arg(".jpg");
                    imagecount++;
                    }
                    else
                    {

                        train_xml("at.txt");
                        imagecount = 0;
                        collectstart = false;
                        ui->ImageShow->clear();
                        ui->ImageShow->setVisible(false);
                        ui->nextButton->setText("下一步");
                        timer->stop();
                        cap.release();
                        menuSelect(1);
                    }
                }
                else
                {
                    folder.mkdir(QString("%1/faceDB/%2").arg(QDir::currentPath()).arg(QString::number(face_ID)));
                }
            }

    }
}

void Widget::addCustomerDB()
{
    QSqlQuery query(db);
    QString type = QString::number(ui->comboType->currentIndex()+1);

    QString str = QString("select *from combotype where servicetype = '%1'").arg(type);
    query.exec(str);
    int times = 0;
    int lastime = 0;
    int money = 0;
    while(query.next())
    {
        times = query.value("times").toInt();
        lastime = query.value("time").toInt();
        money = query.value("money").toInt();
    }

    if(type == "3")
    {
        QString inmoney = ui->timesEdit->text();
        int surplusnum = inmoney.toInt()/money;
        QString str1 = QString("insert into orderform(face_ID, name,time,operation,money,servicetype) values('%1', '%2' ,'%3','注册','%4','3')").arg(QString::number(face_ID)).arg(name).arg(QDateTime::currentDateTime().toString("yyyy/M/d hh:mm:ss")).arg(inmoney);
        QString str5 = QString("insert into customer(face_ID,name,telNum,serviceType,surplusNum,deadline,enable) values('%1','%2','%3','%4','%5','%6','%7')").arg(QString::number(face_ID)).arg(name).arg(telnum).arg(type).arg(QString::number(surplusnum)).arg("").arg("1");
        query.exec(str1);
        query.exec(str5);
    }
    else
    {
        if(times == 0)
        {
            QString str3 = QString("insert into orderform(face_ID, name,time,operation,money,servicetype) values('%1', '%2' ,'%3','注册','%4','%5')").arg(face_ID).arg(name).arg(QDateTime::currentDateTime().toString("yyyy/M/d hh:mm:ss")).arg(QString::number(money)).arg(type);
            QString str6 = QString("insert into customer(face_ID,name,telNum,serviceType,surplusNum,deadline,enable) values('%1','%2','%3','%4','%5','%6','%7')").arg(QString::number(face_ID)).arg(name).arg(telnum).arg(type).arg("").arg(QDate::currentDate().addYears(lastime).toString("yyyy/M/d")).arg("1");
            query.exec(str3);
            query.exec(str6);
        }
        else
        {
            QString str4 = QString("insert into orderform(face_ID, name,time,operation,money,servicetype) values('%1', '%2' ,'%3','注册','%4','%5')").arg(face_ID).arg(name).arg(QDateTime::currentDateTime().toString("yyyy/M/d hh:mm:ss")).arg(QString::number(money)).arg(type);
            QString str7 = QString("insert into customer(face_ID,name,telNum,serviceType,surplusNum,deadline,enable) values('%1','%2','%3','%4','%5','%6','%7')").arg(QString::number(face_ID)).arg(name).arg(telnum).arg(type).arg(QString::number(times)).arg("").arg("1");
            query.exec(str4);
            query.exec(str7);
        }
    }
}
void Widget::searchSportData()
{
    if(ui->nameSearch->text() != NULL)
    {
    menuSelect(10);
    ui->searchWidget->setVisible(true);
    QString name  = ui->nameSearch->text();
    if(ui->faceidSearch->text() != NULL)
    {
        face_ID = ui->faceidSearch->text().toInt();
        return;
    }
    QSqlQuery query;
    QString commend = QString("select * from customer WHERE name = '%1'").arg(name);
    if(!query.exec(commend))
    {
        qDebug() << query.lastError();
    }
    else
    {
        while (query.next())
        {
            face_ID = query.value(0).toInt();
        }
    }
    }
    else if(ui->faceidSearch->text() != NULL)
    {
        face_ID = ui->faceidSearch->text().toInt();
        menuSelect(10);
        ui->searchWidget->setVisible(true);
        ui->searchWidget->setCurrentRow(0);
        searchDataType();

    }

}
/*
void Widget::searchSportData()
{
    if(ui->nameSearch->text() != NULL)
    {
    tableClearAll();
    QString startime = ui->start_dateTime->text();
    QString endtime = ui->end_dateTime->text();
    QString name  = ui->nameSearch->text();

    QSqlQuery query;
    QString commend = QString("select * from customer WHERE name = '%1'").arg(name);
    if(!query.exec(commend))
    {
        qDebug() << query.lastError();
    }
    else
    {
        while (query.next())
        {
            QString faceID = query.value(0).toString();
            QSqlQuery sql_query;
            QString select_all_sql = "select * from sportdata WHERE time between:detectTime and:endTime and face_ID = :name";
            sql_query.prepare(select_all_sql);
            sql_query.bindValue(":detectTime", startime);
            sql_query.bindValue(":endTime", endtime);
            sql_query.bindValue(":name", faceID);
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
                    QString datetime = sql_query.value(3).toString();
                    int place = sql_query.value(4).toInt();
                    int heart_beats = sql_query.value(5).toInt();
                    float temperature = sql_query.value(6).toFloat();

                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(ID)));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(QString::number(parent_ID)));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(QString::number(current_ID)));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(datetime));
                    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(QString::number(place)));
                    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(QString::number(heart_beats)));
                    ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem(QString("%1").arg(temperature)));
                }
        }
        }
    }
    }
}
*/
void Widget::searchDataType()
{
    QSqlQuery query;
    QString commend1 = QString("select * from customer WHERE face_ID = '%1'").arg(face_ID);
    QString commend2 = QString("select * from orderform WHERE face_ID = '%1'").arg(face_ID);
    QString lastime,logintime;

    if(ui->dirbar->count() == 3)
    {
        ui->dirbar->addItem(" >客户查询 ");
    }

    switch (ui->searchWidget->currentRow()) {
    case 0:
            setMenulastVisable(false);
            ui->searchWidget->setVisible(true);
            ui->tableWidget->setVisible(false);
            query.exec(commend1);
            while (query.next())
            {
                name = query.value(1).toString();
                telnum = query.value(2).toString();
                if(query.value(4).toInt() == 0)
                {
                    lastime = query.value(5).toString();
                }
                else
                {
                    lastime = query.value(4).toString();
                }
            }
            if(name == NULL)
            {
                menuSelect(2);
                QMessageBox::warning(this,tr("警告"),"查无此人",QMessageBox::Ok);
                return;
            }
            query.clear();
            query.exec(commend2);
            while (query.next())
            {
                 if(query.value(3).toString() == tr("认证"))
                     {
                     logintime = query.value(2).toString();
                     }

            }
            ui->customlabel->setVisible(true);
            ui->customlabel_2->setVisible(true);
            ui->customlabel_3->setVisible(true);
            ui->customlabel_4->setVisible(true);
            ui->customlabel_5->setVisible(true);
            ui->customlabel_6->setVisible(true);

            ui->customlabel->setText(QString("姓名：%1").arg(name));
            ui->customlabel_2->setText(QString("编号：%1").arg(QString::number(face_ID)));
            ui->customlabel_3->setText(QString("电话：%1").arg(telnum));
            ui->customlabel_4->setText(QString("套餐：%1(次/时间)").arg(lastime));
            ui->customlabel_5->setText(QString("上次登陆时间：%1").arg(logintime));
            ui->customlabel_6->setPixmap(QPixmap::fromImage(QImage(QString("%1/faceDB/%2/0.jpg").arg(QDir::currentPath()).arg(QString::number(face_ID)))));

            ui->dirbar->item(3)->setText(" >客户查询 ");

        break;
    case 1:
        setMenulastVisable(false);
        ui->searchWidget->setVisible(true);
        ui->end_dateTime->setVisible(true);
        ui->start_dateTime->setVisible(true);
        ui->label_9->setVisible(true);
        ui->label_10->setVisible(true);
        ui->showType->setVisible(true);
        ui->showType->setCurrentIndex(0);
        changeShowType();

        ui->dirbar->item(3)->setText(" >运动数据 ");

        break;

    case 2:
        setMenulastVisable(false);
        ui->searchWidget->setVisible(true);
        ui->end_dateTime->setVisible(true);
        ui->start_dateTime->setVisible(true);
        ui->label_9->setVisible(true);
        ui->label_10->setVisible(true);
        ui->loginType->setVisible(true);
        changeLoginType();

        ui->dirbar->item(3)->setText(" >登陆记录 ");

        break;

    default:
        break;
    }
}

void Widget::changeShowType()
{
    if(ui->showType->currentIndex() == 0)
    {
        ui->routeEdit->setVisible(false);
        ui->routeButton->setVisible(false);
        ui->placewidget->setVisible(false);
        ui->drawwidget->setVisible(false);
        ui->label_16->setVisible(false);
        ui->label_17->setVisible(false);
        ui->label_18->setVisible(false);
        ui->label_19->setVisible(false);
        ui->label_20->setVisible(false);
        ui->label_21->setVisible(false);
        ui->label_22->setVisible(false);
        ui->label_23->setVisible(false);
        ui->customerdatalist->setVisible(false);
        ui->label_24->setVisible(false);


        customerdata.clear();
        customerdatelist.clear();
        QList<int> temp1;
        QList<float> temp2;

        cardcount = 0;
        ui->tableWidget->setVisible(true);
        ui->tableWidget->setGeometry(190,160,911,600);
        tableClearAll();
        QStringList headers;
        headers << ("face_ID") << ("parent_ID") << ("current_ID") << ("time") << ("palce") << ("心率")<<("体温");
        ui->tableWidget->setColumnCount(7);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0,125);
        ui->tableWidget->setColumnWidth(1, 100);
        ui->tableWidget->setColumnWidth(2, 100);
        ui->tableWidget->setColumnWidth(3, 150);
        ui->tableWidget->setColumnWidth(4, 100);
        ui->tableWidget->setColumnWidth(5, 100);
        ui->tableWidget->setColumnWidth(6, 100);
        ui->tableWidget->setFixedWidth(810);
        QString select_all_sql = QString("select * from sportdata WHERE face_ID = '%1'").arg(face_ID);
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
                    QString datetime = sql_query.value(3).toString();
                    int place = sql_query.value(4).toInt();
                    int heart_beats = sql_query.value(5).toInt();
                    float temperature = sql_query.value(6).toFloat();
                    QList<float> temp;
                    temp.append(sql_query.value(5).toFloat());
                    temp.append(sql_query.value(6).toFloat());
                    temp.append(sql_query.value(4).toFloat());
                    temp1.append(sql_query.value(5).toInt());
                    temp2.append(sql_query.value(6).toFloat());

                    customerdata.insert(QDateTime::fromString(datetime,"yyyy/M/d h:mm:ss"),temp);
                    customerdatelist.append(QDateTime::fromString(datetime,"yyyy/M/d h:mm:ss"));


                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(ID)));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(QString::number(parent_ID)));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(QString::number(current_ID)));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(datetime));
                    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(QString::number(place)));
                    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(QString::number(heart_beats)));
                    ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem(QString("%1").arg(temperature)));

                }
            }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
        qSort(customerdatelist.begin(),customerdatelist.end(),compareBarData);
        qSort(temp1.begin(),temp1.end());
        qSort(temp2.begin(),temp2.end());
        int he = 0;
        float tp =0;
        for(int i = 0;i<temp1.count();i++)
        {
            he +=temp1[i];
            tp +=temp2[i];
        }
        if(temp1.count() == 0)
            return;
        he = he/temp1.count();
        tp = tp/temp1.count();
        ui->label_17->setText(QString("最大值：%1").arg(QString::number(temp2[temp2.count()-1])));
        ui->label_19->setText(QString("最小值：%1").arg(QString::number(temp2[0])));
        ui->label_18->setText(QString("平均值：%1").arg(QString::number(tp)));
        ui->label_21->setText(QString("最大值：%1").arg(QString::number(temp1[temp2.count()-1])));
        ui->label_23->setText(QString("最小值：%1").arg(QString::number(temp1[0])));
        ui->label_22->setText(QString("平均值：%1").arg(QString::number(he)));


    }
    else if(ui->showType->currentIndex() == 1)
    {
        ui->routeEdit->setVisible(false);
        ui->routeButton->setVisible(false);
        ui->label_24->setVisible(false);
        ui->customerdatalist->setVisible(false);
        ui->label_16->setVisible(true);
        ui->label_17->setVisible(true);
        ui->label_18->setVisible(true);
        ui->label_19->setVisible(true);
        ui->label_20->setVisible(true);
        ui->label_21->setVisible(true);
        ui->label_22->setVisible(true);
        ui->label_23->setVisible(true);

        ui->tableWidget->setVisible(false);
        ui->placewidget->setVisible(false);
        heartseries->clear();
        teptseries->clear();
        ui->drawwidget->setVisible(true);
        drawtimer->start(1000);

    }
    else
    {
        ui->drawwidget->setVisible(false);
        ui->label_16->setVisible(false);
        ui->label_17->setVisible(false);
        ui->label_18->setVisible(false);
        ui->label_19->setVisible(false);
        ui->label_20->setVisible(false);
        ui->label_21->setVisible(false);
        ui->label_22->setVisible(false);
        ui->label_23->setVisible(false);
        ui->tableWidget->setVisible(false);
        ui->placewidget->setVisible(true);
        ui->customerdatalist->setVisible(true);
        ui->label_24->setVisible(true);
        ui->routeEdit->setVisible(true);
        ui->routeButton->setVisible(true);
        ui->customerdatalist->clear();
        trackseries->clear();
        for(int i = 0;i<customerdata.count();i++)
        {
            trackseries->append(mapoint->at((int)customerdata.value(customerdatelist.at(i))[2]).toPoint());
            ui->customerdatalist->append("");
            ui->customerdatalist->append(QString("时间：%1\n地点：place%2,心率：%3,体温：%4").arg(customerdatelist.at(i).toString("yyyy/M/d hh:mm:ss")).arg(QString::number(customerdata.value(customerdatelist.at(i))[2])).arg(QString::number(customerdata.value(customerdatelist.at(i))[0])).arg(QString::number(customerdata.value(customerdatelist.at(i))[1])));
            ui->customerdatalist->append("");
        }


    }
}
void Widget::changeLoginType()
{
    ui->tableWidget->setVisible(true);
    ui->tableWidget->setGeometry(190,160,911,550);
    tableClearAll();
    if(ui->loginType->currentIndex() == 0)
    {
        ui->loginChart->setVisible(false);
        ui->loginEdit->setVisible(false);
        QStringList headers;
        headers << ("face_ID") << ("name") << ("time") << ("operation") << ("money") << ("serviceType");
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0,125);
        ui->tableWidget->setColumnWidth(1, 100);
        ui->tableWidget->setColumnWidth(2, 200);
        ui->tableWidget->setColumnWidth(3, 150);
        ui->tableWidget->setColumnWidth(4, 100);
        ui->tableWidget->setColumnWidth(5, 100);
        ui->tableWidget->setFixedWidth(810);
        QString select_all_sql = QString("select * from orderform WHERE face_ID = '%1' and operation = '注册'").arg(face_ID);
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

                    QString name = sql_query.value(1).toString();
                    QString datetime = sql_query.value(2).toString();
                    QString operation = sql_query.value(3).toString();
                    QString money = sql_query.value(4).toString();
                    QString type = sql_query.value(5).toString();

                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(face_ID)));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(name));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(datetime));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(operation));
                    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(money));
                    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(type));

                }
            }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    }
    else
    {
        ui->loginEdit->setVisible(true);
        ui->loginChart->setVisible(true);
        QStringList headers;
        headers << ("face_ID") << ("name") << ("time") << ("operation");
        ui->tableWidget->setColumnCount(4);
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setHidden(false);//设置表头可见
        ui->tableWidget->setShowGrid(true);
        ui->tableWidget->verticalHeader()->setHidden(true);
        ui->tableWidget->setColumnWidth(0,125);
        ui->tableWidget->setColumnWidth(1, 100);
        ui->tableWidget->setColumnWidth(2, 200);
        ui->tableWidget->setColumnWidth(3, 150);
        ui->tableWidget->setFixedWidth(610);
        QString select_all_sql = QString("select * from orderform WHERE face_ID = '%1' and operation = '认证'").arg(face_ID);
        QSqlQuery sql_query;//QSqlQuery类提供执行和操作的SQL语句的方法

            //查询所有记录
            //sql_query.prepare(select_all_sql);
            if (!sql_query.exec(select_all_sql))
            {
                qDebug() << sql_query.lastError();//查询失败
            }
            else
            {
                //统计容器初始化
                QList<float> temp;
                for(int i = 0;i<QDateTime::currentDateTime().date().month();i++)
                {
                    temp.append(0);
                }
                //检索下一个
                while (sql_query.next())
                {

                    QString name = sql_query.value(1).toString();
                    QString datetime = sql_query.value(2).toString();
                    QString operation = sql_query.value(3).toString();

                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowCount);
                    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString::number(face_ID)));
                    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(name));
                    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(datetime));
                    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem(operation));

                    //画图统计
                    QDateTime x = QDateTime::fromString(datetime,"yyyy/M/d h:mm:ss");
                    temp[x.date().month()-1] += 1;

                }
                loginseries->clear();
                for(int i = 0; i < QDateTime::currentDateTime().date().month();i++)
                {
                    loginseries->append(QString("月份：%1").arg(i+1),temp[i]/ui->tableWidget->rowCount());
                }
                ui->loginEdit->clear();
                for(int i = 0; i < QDateTime::currentDateTime().date().month();i++)
                {
                    ui->loginEdit->append(QString("%1月份来访次数：%2, 占比：%3").arg(i+1).arg(QString::number(temp[i])).arg(QString::number(temp[i]/ui->tableWidget->rowCount())));
                }
            }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑

    }
}

void Widget::on_refreshCustom_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("修改的客户:"));
    // Value1
    QString value1 = QString("face_ID:");
    QLineEdit *spinbox1 = new QLineEdit(&dialog);
    spinbox1->setPlaceholderText("可以不填");
    form.addRow(value1, spinbox1);
    // Value2
    QString value2 = QString("姓名:");
    QLineEdit *spinbox2 = new QLineEdit(&dialog);
    form.addRow(value2, spinbox2);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted)
    {
        ui->tableWidget->setVisible(false);
        QString faceID = spinbox1->text();
        QString na = spinbox2->text();
        if(faceID == NULL)
        {
            getfaceID(na);
        }
        else
        {
            face_ID = faceID.toInt();
        }

        QSqlQuery query;
        QString commend = QString("select * from customer WHERE face_ID = '%1'").arg(QString::number(face_ID));
        if(!query.exec(commend))
        {
            qDebug() << query.lastError();
        }
        else
        {
            while (query.next())
            {
                name = query.value(1).toString();
                telnum = query.value(2).toString();
                surplusnum = query.value(4).toInt();
                deadtime = QDateTime::fromString(query.value(5).toString(),"yyyy/M/d hh:mm:ss");
            }
        }

        ui->comboType->clear();
        QString str2 = QString("select *from combotype");
        query.exec(str2);
        while(query.next())
        {
            ui->comboType->addItem(query.value("servicetype").toString());
        }

        ui->refreshCustom->setVisible(false);
        ui->addNewCustom->setVisible(false);
        setInfiedVisable(true);
        ui->customlabel_6->setVisible(true);
        ui->nextButton->setVisible(false);
        ui->addmoneyCustom->setVisible(true);
        ui->fixCustom->setVisible(true);
        ui->freezeCustom->setVisible(true);
        ui->deleteCustom->setVisible(true);
        ui->customlabel_6->setPixmap(QPixmap::fromImage(QImage(QString("%1/faceDB/%2/0.jpg").arg(QDir::currentPath()).arg(QString::number(face_ID)))));
        ui->nameEdit->setText(na);
        ui->telEdit->setText(telnum);



    }
}

void Widget::getfaceID(QString na)
{
    QSqlQuery query;
    QString commend = QString("select * from customer WHERE name = '%1'").arg(na);
    if(!query.exec(commend))
    {
        qDebug() << query.lastError();
    }
    else
    {
        while (query.next())
        {
            face_ID = query.value(0).toInt();
        }
    }
}

void Widget::on_fixCustom_clicked()
{
    QSqlQuery query;
    qDebug()<<ui->telEdit->text()<<face_ID;
    QString temp = QString("update customer set telNum = '%1',name = '%2' where face_ID = '%3'").arg(ui->telEdit->text()).arg(ui->nameEdit->text()).arg(QString::number(face_ID));
    if(query.exec(temp))
    {
         QMessageBox::warning(this,tr("提示"),tr("修改成功"),QMessageBox::Ok);
    }
}

void Widget::on_addmoneyCustom_clicked()
{
    QSqlQuery query(db);
    QString type = ui->comboType->currentText();

    QString str = QString("select *from combotype where servicetype = '%1'").arg(type);
    query.exec(str);
    int times = 0;
    int lastime = 0;
    int money = 0;
    while(query.next())
    {
        times = query.value("times").toInt();
        lastime = query.value("time").toInt();
        money = query.value("money").toInt();
    }

    if(type == "3")
    {
        QString inmoney = ui->timesEdit->text();
        int sur = inmoney.toInt()/money;
        QString str1 = QString("insert into orderform(face_ID, name,time,operation,money,servicetype) values('%1', '%2' ,'%3','注册','%4','3')").arg(QString::number(face_ID)).arg(name).arg(QDateTime::currentDateTime().toString("yyyy/M/d hh:mm:ss")).arg(inmoney);
        QString str5 = QString("update customer set surplusNum = '%1' where face_ID = '%2'").arg(QString::number(surplusnum+sur)).arg(QString::number(face_ID));
        query.exec(str1);
        query.exec(str5);
    }
    else
    {
           QString str3 = QString("insert into orderform(face_ID, name,time,operation,money,servicetype) values('%1', '%2' ,'%3','注册','%4','%5')").arg(face_ID).arg(name).arg(QDateTime::currentDateTime().toString("yyyy/M/d hh:mm:ss")).arg(QString::number(money)).arg(type);
           QString str6 = QString("update customer set deadline = '%1' where face_ID = '%2'").arg(deadtime.addYears(lastime).toString("yyyy/M/d hh:mm:ss")).arg(QString::number(face_ID));
           query.exec(str3);
           query.exec(str6);
           QString str7 = QString("update customer set surplusNum = '%1' where face_ID = '%2'").arg(QString::number(surplusnum+times)).arg(QString::number(face_ID));
           query.exec(str7);
    }
    QMessageBox::warning(this,tr("提示"),tr("充值成功"),QMessageBox::Ok);
}

void Widget::on_freezeCustom_clicked()
{
    if(ui->freezeCustom->text() == tr("冻结"))
    {
        //QMessageBox::warning(this,tr("提示"),tr("冻结"),QMessageBox::Ok);
        ui->freezeCustom->setText("解冻");
    }
    else
    {
        //QMessageBox::warning(this,tr("提示"),tr("解冻"),QMessageBox::Ok);
        ui->freezeCustom->setText("冻结");
    }
}

void Widget::DrawRoute()
{
    if(routecount<customerdatelist.count())
    {
    trackseries->append(mapoint->at((int)customerdata.value(customerdatelist.at(routecount)).at(2)).toPoint());
    routecount++;
    }
    else
    {
        routecount = 0;
        routetimer->stop();
    }
}

void Widget::DrawLine()
{

    if(cardcount == 0)
    {
        //设置坐标轴显示范围
        //axisX->setMin(customerdatelist.at(cardcount));       //系统当前时间的前一秒
        //axisX->setMax(customerdatelist.at(cardcount+4));            //系统当前时
        axisX->setMin(cardcount);
        axisX->setMax(cardcount+5);
        for(int i = 0;i<5;i++)
        {
            heartseries->append(i, customerdata.value(customerdatelist.at(cardcount+i)).at(0));
            teptseries->append(i, customerdata.value(customerdatelist.at(cardcount+i)).at(1));
        }
        cardcount++;
        return;
    }

    if(cardcount+4 < customerdatelist.count())
    {
        //设置坐标轴显示范围

        //axisX->setMin(customerdatelist.at(cardcount));       //系统当前时间的前一秒
        //axisX->setMax(customerdatelist.at(cardcount+4));            //系统当前时
        //变更坐标轴范围
        axisX->setMin(cardcount);
        axisX->setMax(cardcount+5);

        //增加新的点到曲线末端
        heartseries->append(cardcount+4, customerdata.value(customerdatelist.at(cardcount+4)).at(0));
        teptseries->append(cardcount+4, customerdata.value(customerdatelist.at(cardcount+4)).at(1));
        //qDebug()<<customerdatelist.at(cardcount+4).toString("yyyy/M/D hh-mm-ss")<<customerdata.value(customerdatelist.at(cardcount+4)).at(0)<<customerdata.value(customerdatelist.at(cardcount+4)).at(1);
        cardcount++;
        return;
    }
    drawtimer->stop();

}
void Widget::deviceMenuSelect()
{
    switch (ui->deviceMenu->currentRow()) {
    case 0:
        ui->label_2->setText("前台设备编号");
        ui->label_5->setText("线程号");
        ui->label_3->setText("状态");
        ui->label_4->setText("网络");
        menuSelect(3);
        deviceTableClear();
        break;
    case 1:
        menuSelect(3);
        ui->label_2->setText("后端设备编号");
        ui->label_5->setText("IP");
        ui->label_3->setText("端口");
        ui->label_4->setText("状态");
        deviceTableClear();
        break;
    case 2:
        menuSelect(10);
        ui->deviceMenu->setVisible(true);
        ui->nameEdit2->setVisible(true);
        ui->namelabel->setVisible(true);
        ui->deviceEdit->setVisible(true);
        ui->devicelabel->setVisible(true);
        ui->placewidget->setVisible(true);
        ui->deviceSearch->setVisible(true);


        break;
    default:
        break;
    }
}
void Widget::deviceTableClear()
{
    ui->IPManager->clear();
    ui->NetSpeed->clear();
    ui->TcpState->clear();
    ui->ThreadNum->clear();
}

void Widget::deviceOnPoint(const QPointF &point, bool state)
{
    if (state) {
        for(int i = 0;i<mapoint->count();i++)
        {
            if(point == mapoint->points().at(i))
            {
                 // m_valueLabel.at(i)->setStyleSheet(QString("QLabel{color:#1564FF; font-family:\"Microsoft Yahei\"; font-size:12px; font-weight:bold;"
                   //                                          " background-color:rgba(21, 255, 255, 51); border-radius:4px; text-align:center;}"));
                label->setText(QString("palce:%1").arg(QString::number(i)));
                QPoint curPos = mapFromGlobal(QCursor::pos());
                label->move(curPos.x() - label->width() / 2, curPos.y() - label->height() * 1.5);//移动数值
                label->show();//显示出来
                break;
            }
        }
    }
    else {
        label->hide();
        /*for(int i = 0;i<mapoint->count();i++)
        {

           //m_valueLabel.at(i)->setStyleSheet(QString("QLabel{color:#1564FF; font-family:\"Microsoft Yahei\"; font-size:12px; font-weight:bold;"
             //                                                " background-color:rgba(21, 100, 255, 51); border-radius:4px; text-align:center;}"));

        }*/
    }
}

void Widget::devicePlaceChoose(const QPointF &point)
{
    if(ui->routeEdit->isVisible())
    {
        ui->routeEdit->clear();
        int i = 0;
        for(;i<mapoint->count();i++)
        {
            if(point == mapoint->at(i))
                break;
        }
        ui->routeEdit->append(QString("place%1打卡信息:\n").arg(QString::number(i)));
        for(int j = 0;j < customerdatelist.count();j++)
        {
            if((int)customerdata.value(customerdatelist.at(j)).at(2) == i)
            {
                ui->routeEdit->append(QString("时间：%1, 地点：place%2, 心率：%3, 体温：%4").arg(customerdatelist.at(j).toString("yyyy/M/d hh:mm:ss")).arg(QString::number(customerdata.value(customerdatelist.at(j))[2])).arg(QString::number(customerdata.value(customerdatelist.at(j))[0])).arg(QString::number(customerdata.value(customerdatelist.at(j))[1])));
                ui->routeEdit->append("");
            }
        }
        ui->routeEdit->verticalScrollBar()->setValue(0);
        MyPlaceX *x = new MyPlaceX(customerdata,customerdatelist,i);
        x->show();
    }
    else
    {
        trackseries->clear();
    }
}

void Widget::on_routeButton_clicked()
{
    trackseries->clear();
    routetimer->start(500);
}

void Widget::on_orderAnalysis_clicked()
{
    ui->label_28->setVisible(false);
    ui->comboType2->setVisible(false);
    ui->label_30->setVisible(false);
    ui->label_29->setVisible(false);
    ui->nameSearch2->setVisible(false);
    ui->faceidSearch2->setVisible(false);
    ui->searchButton2->setVisible(false);
    ui->tableWidget->setVisible(false);
    ui->accountChart->setVisible(true);
    ui->comboChart->setVisible(true);
    ui->ordershow2->setVisible(true);
    ui->ordershow3->setVisible(true);
    ui->accountChart->setGeometry(10,150,450,400);
    ui->comboChart->setGeometry(460,150,460,400);
    ui->ordershow2->setGeometry(10,600,450,200);
    ui->ordershow3->setGeometry(460,600,450,200);
    ui->accountChart->chart()->series().clear();
    ui->accountChart->chart()->removeSeries(ui->accountChart->chart()->series().at(1));
    ui->accountChart->chart()->removeSeries(ui->accountChart->chart()->series().at(0));
    ui->accountChart->chart()->removeAxis(ui->accountChart->chart()->axisX());
    ui->accountChart->chart()->removeAxis(ui->accountChart->chart()->axisY());
    comboseries->clear();
    //dbList("orderform");

    QBarSet *accountset = new QBarSet("销售额");
    QBarSet *countset = new QBarSet("到访人数");
    QList<qreal> temp1;//销售额
    QList<qreal> temp2;//到访人数
    QList<float>  temp3;//套餐售卖情况
    QStringList str;

    QString select_all_sql = "select * from combotype";
    QSqlQuery sql_query;//QSqlQuery类提供执行和操作的SQL语句的方法
    sql_query.exec(select_all_sql);
    while (sql_query.next())
    {
        temp3.append(0);
    }
    temp3.append(0);

    for (int i = 1;i <= QDateTime::currentDateTime().date().month(); i++ )
    {
        temp1.append(0);
        temp2.append(0);
        str.append(QString("%1月").arg(QString::number(i)));
    }
    for(int i = 0; i<ui->tableWidget->rowCount(); i++)
    {


        int m = QDateTime::fromString(ui->tableWidget->item(i,2)->text(),"yyyy/M/d hh:mm:ss").date().month();;
        if(ui->tableWidget->item(i,3)->text() == tr("注册"))
        {
            temp1[m-1] += ui->tableWidget->item(i,4)->text().toInt();
            temp3[ui->tableWidget->item(i,5)->text().toInt()-1]++;
            temp3[temp3.count()-1]++;

        }
        else
        {
            temp2[m-1]++;
        }
    }

    accountset->append(temp1);
    countset->append(temp2);
    qSort(temp1.begin(),temp1.end());
    qSort(temp2.begin(),temp2.end());

    QBarSeries* barSerise1 = new QBarSeries();
    QBarSeries* barSerise2 = new QBarSeries();
    barSerise1->append(accountset);
    barSerise2->append(countset);
    ui->accountChart->chart()->addSeries(barSerise1);
    ui->accountChart->chart()->addSeries(barSerise2);
    ui->accountChart->chart()->createDefaultAxes();

    QValueAxis* axisy1 = new QValueAxis();
    axisy1->setRange(temp1[0],temp1[temp1.count()-1]);
    ui->accountChart->chart()->addAxis(axisy1,Qt::AlignLeft);
    ui->accountChart->chart()->setAxisY(axisy1,barSerise1);

    QValueAxis* axisy2 = new QValueAxis();
    axisy2->setRange(temp2[0],temp2[temp2.count()-1]);
    ui->accountChart->chart()->addAxis(axisy2,Qt::AlignRight);
    ui->accountChart->chart()->setAxisY(axisy2,barSerise2);
    QBarCategoryAxis* axisx2 = new QBarCategoryAxis();
    axisx2->append(str);
    ui->accountChart->chart()->setAxisX(axisx2);

    ui->ordershow3->clear();
    for(int i = 0; i<temp3.count()-1; i++)
    {
        comboseries->append(QString("套餐%1").arg(QString::number(i+1)),temp3[i]/temp3[temp3.count()-1]);
        ui->ordershow3->append(QString("套餐%1所占份额:%2").arg(QString::number(i+1)).arg(QString::number(temp3[i]/temp3[temp3.count()-1])));
    }
    ui->ordershow2->clear();
    for(int i = 0; i<str.count(); i++)
    {
        ui->ordershow2->append(QString("%1月销售额：%2 元 ; 到访人数：%3 人次").arg(QString::number(i+1)).arg(QString::number(temp1[i])).arg(QString::number(temp2[i])));
    }


}

void Widget::on_orderList_clicked()
{
    ui->label_28->setVisible(false);
    ui->label_29->setVisible(false);
    ui->nameSearch2->setVisible(false);
    ui->faceidSearch2->setVisible(false);
    ui->searchButton2->setVisible(false);
    ui->comboType2->setVisible(false);
    ui->label_30->setVisible(false);
    dbList("orderform");
    ui->tableWidget->setGeometry(0,160,811,600);
    ui->accountChart->setVisible(false);
    ui->comboChart->setVisible(false);
    ui->ordershow2->setVisible(false);
    ui->ordershow3->setVisible(false);
    ui->tableWidget->setVisible(true);
}

void Widget::on_orderSearch_clicked()
{
    ui->accountChart->setVisible(false);
    ui->comboChart->setVisible(false);
    ui->ordershow2->setVisible(false);
    ui->ordershow3->setVisible(false);
    ui->tableWidget->setVisible(true);
    ui->label_28->setVisible(true);
    ui->label_29->setVisible(true);
    ui->nameSearch2->setVisible(true);
    ui->faceidSearch2->setVisible(true);
    ui->searchButton2->setVisible(true);
    ui->comboType2->setVisible(true);
    ui->label_30->setVisible(true);
    ui->tableWidget->setGeometry(0,260,911,400);

}

void Widget::on_searchButton2_clicked()
{
    if(ui->faceidSearch2->text() == NULL)
    {
        if(ui->nameSearch2->text() == NULL)
        {
            QMessageBox::warning(this,tr("警告"),tr("请输入完整信息"),QMessageBox::Ok);
            return;
        }
        else
        {
             QString name  = ui->nameSearch2->text();
             QSqlQuery query;
             QString commend = QString("select * from customer WHERE name = '%1'").arg(name);
             if(!query.exec(commend))
             {
                 qDebug() << query.lastError();
             }
             else
             {
                 while (query.next())
                 {
                     face_ID = query.value(0).toInt();
                 }
             }

        }
    }
    else
    {
        face_ID = ui->faceidSearch2->text().toInt();
    }
    changeOrderTime();

}

void Widget::on_comboType2_currentIndexChanged(int index)
{
    changeOrderTime();
}

void Widget::on_newOrderButton_clicked()
{
    ui->tableWidget->setVisible(false);
    for(int i = 0;i<menuorder.count();i++)
    {
        menuorder[i]->setVisible(true);
    }
    ui->orderEdit1->clear();
    ui->orderEdit2->clear();
    ui->orderEdit3->clear();
    ui->orderEdit4->clear();

}

void Widget::on_orderbutton_clicked()
{
    QSqlQuery query(db);
    QString str = QString("select *from combotype");
    query.exec(str);
    int servicetype = 0;
    while(query.next())
    {
        servicetype = query.value("servicetype").toInt();
    }
    servicetype++;
    query.clear();
    QString str1 = QString("insert into combotype(servicetype,money,time,times,supply) values('%1', '%2' ,'%3','%4','%5')").arg(QString::number(servicetype)).arg(ui->orderEdit1->text()).arg(ui->orderEdit3->text()).arg(ui->orderEdit2->text()).arg(ui->orderEdit4->toPlainText());
    query.exec(str1);
    QMessageBox::warning(this,"提示","插入成功",QMessageBox::Ok);
    menuSelect(0);
}

void Widget::on_deviceSearch_clicked()
{
    if(ui->deviceEdit->text() == NULL && ui->nameEdit2->text() == NULL)
    {
        QMessageBox::warning(this,"警告","请输入信息",QMessageBox::Ok);
        return;
    }
    QString faid;
    if(ui->deviceEdit->text() == NULL)
    {
        QString na = ui->nameEdit2->text();
        for(int i = 0;i<DeviceList.count();i++)
        {
            if(DeviceList.values()[i][2] == na)
            {
                faid = DeviceList.values()[i][3];
                break;
            }
        }

    }
    else
    {
        if(DeviceList.contains(ui->deviceEdit->text()))
            faid = DeviceList.value(ui->deviceEdit->text())[3];
    }
    if(faid == NULL)
    {
        QMessageBox::warning(this,"警告","请输入正确信息",QMessageBox::Ok);
        return;
    }
    QSqlQuery query(db);
    QString str1 = QString("select * from sportdata where face_ID = '%1'").arg(faid);
    query.exec(str1);
    QHash<QDateTime,int> placelist;
    QList<QDateTime> datelist;
    while(query.next())
    {
        QStringList temp;
        QStringList temp2;
        temp = query.value(3).toString().split("/");
        temp2 = QString(temp.at(2)).split(" ");
        if(QString(temp.at(1)).toInt() == QDate::currentDate().month() && QString(temp2.at(0)).toInt() == QDate::currentDate().day())
        {
            placelist.insert(QDateTime::fromString(query.value(3).toString(),"yyyy/M/d hh:mm:ss"),query.value(4).toInt());
            datelist.append(QDateTime::fromString(query.value(3).toString(),"yyyy/M/d hh:mm:ss"));
        }
    }
    qSort(datelist.begin(),datelist.end(),compareBarData);
    QMessageBox::warning(this,"提示",QString("当前设备的位置：%1").arg(QString::number(placelist.value(datelist.at(datelist.count()-1)))),QMessageBox::Ok);

}

void Widget::on_newAccount_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("新增账号:"));
    // Value1
    QString value1 = QString("身份:");
    QComboBox *spinbox1 = new QComboBox(&dialog);
    spinbox1->addItem("管理员");
    spinbox1->addItem("员工");
    form.addRow(value1, spinbox1);
    // Value2
    QString value2 = QString("电话:");
    QLineEdit *spinbox2 = new QLineEdit(&dialog);
    form.addRow(value2, spinbox2);
    // Value3
    QString value3 = QString("password:");
    QLineEdit *spinbox3 = new QLineEdit(&dialog);
    form.addRow(value3, spinbox3);
    // Value4
    QString value4 = QString("姓名:");
    QLineEdit *spinbox4 = new QLineEdit(&dialog);
    form.addRow(value4, spinbox4);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    dialog.resize(400,200);
    dialog.setWindowTitle("注册账号");
    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted)
    {
        QString iden= spinbox1->currentText();
        QString tel = spinbox2->text();
        QString paw = spinbox3->text();
        QString name = spinbox4->text();
        qDebug("here");
        QSqlQuery query(db);
        QString str = QString("insert into administrators(identification,password,user_ID,name,telNum,lastime) values('%1', '%2' ,'%3','%4','%5','%6')").arg(iden).arg(paw).arg(tel).arg(name).arg(tel).arg(QDateTime::currentDateTime().toString("yyyy/M/d hh:mm:ss"));
        if(!query.exec(str))
        {
            qDebug() << query.lastError();
        }
        QMessageBox::warning(this,"提示","注册成功",QMessageBox::Ok);
        dbList("administrators");
    }
}

void Widget::on_refreshAccount_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("修改账号:"));
    // Value5
    QString value5 = QString("账号ID:");
    QLineEdit *spinbox5 = new QLineEdit(&dialog);
    form.addRow(value5, spinbox5);
    // Value1
    QString value1 = QString("身份:");
    QComboBox *spinbox1 = new QComboBox(&dialog);
    spinbox1->addItem("管理员");
    spinbox1->addItem("员工");
    form.addRow(value1, spinbox1);
    // Value2
    QString value2 = QString("电话:");
    QLineEdit *spinbox2 = new QLineEdit(&dialog);
    form.addRow(value2, spinbox2);
    // Value3
    QString value3 = QString("password:");
    QLineEdit *spinbox3 = new QLineEdit(&dialog);
    form.addRow(value3, spinbox3);
    // Value4
    QString value4 = QString("姓名:");
    QLineEdit *spinbox4 = new QLineEdit(&dialog);
    form.addRow(value4, spinbox4);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    dialog.resize(400,200);
    dialog.setWindowTitle("修改账号");
    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted)
    {
        QString iden= spinbox1->currentText();
        QString tel = spinbox2->text();
        QString paw = spinbox3->text();
        QString name = spinbox4->text();
        QString usid = spinbox5->text();
        qDebug("here");
        QSqlQuery query(db);

        QString str = QString("update administrators set identification = '%1',password = '%2',telNum = '%3',name = '%4' where user_ID = '%5'").arg(iden).arg(paw).arg(tel).arg(name).arg(usid);
        if(!query.exec(str))
        {
            qDebug() << query.lastError();
            QMessageBox::warning(this,"提示","账号不存在",QMessageBox::Ok);
        }
        QMessageBox::warning(this,"提示","修改成功",QMessageBox::Ok);
        dbList("administrators");
    }
}

void Widget::on_deletAccount_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("删除账号:"));
    // Value5
    QString value5 = QString("账号ID:");
    QLineEdit *spinbox5 = new QLineEdit(&dialog);
    form.addRow(value5, spinbox5);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    dialog.setWindowTitle("删除账号");
    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted)
    {
        QString usid = spinbox5->text();
        QSqlQuery query(db);
        QString str = QString("delete from administrators where user_ID = '%1'").arg(usid);
        if(!query.exec(str))
        {
            qDebug() << query.lastError();
        }
        QMessageBox::warning(this,"提示","删除成功",QMessageBox::Ok);
        dbList("administrators");
    }
}

void Widget::on_refreshOrderButton_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("修改套餐:"));
    // Value5
    QString value5 = QString("套餐编号:");
    QLineEdit *spinbox5 = new QLineEdit(&dialog);
    form.addRow(value5, spinbox5);
    // Value1
    QString value1 = QString("套餐金额:");
    QLineEdit *spinbox1 = new QLineEdit(&dialog);
    form.addRow(value1, spinbox1);
    // Value2
    QString value2 = QString("次数:");
    QLineEdit *spinbox2 = new QLineEdit(&dialog);
    form.addRow(value2, spinbox2);
    // Value3
    QString value3 = QString("时间:");
    QLineEdit *spinbox3 = new QLineEdit(&dialog);
    form.addRow(value3, spinbox3);
    // Value4
    QString value4 = QString("补充:");
    QTextEdit *spinbox4 = new QTextEdit(&dialog);
    form.addRow(value4, spinbox4);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    dialog.resize(400,300);
    dialog.setWindowTitle("修改套餐");
    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted)
    {
        QString money= spinbox1->text();
        QString times = spinbox2->text();
        QString time = spinbox3->text();
        QString supply = spinbox4->toPlainText();
        QString usid = spinbox5->text();
        QSqlQuery query(db);

        QString str = QString("update combotype set money = '%1',times = '%2',time = '%3',supply = '%4' where servicetype = '%5'").arg(money).arg(times).arg(time).arg(supply).arg(usid);
        if(!query.exec(str))
        {
            qDebug() << query.lastError();
            QMessageBox::warning(this,"提示","套餐不存在",QMessageBox::Ok);
        }
        QMessageBox::warning(this,"提示","修改成功",QMessageBox::Ok);
        dbList("combotype");
    }
}

void Widget::on_deleteOrderButton_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("删除套餐:"));
    // Value5
    QString value5 = QString("套餐编号:");
    QLineEdit *spinbox5 = new QLineEdit(&dialog);
    form.addRow(value5, spinbox5);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    dialog.setWindowTitle("删除套餐");
    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted)
    {
        QString usid = spinbox5->text();
        QSqlQuery query(db);
        QString str = QString("delete from combotype where servicetype = '%1'").arg(usid);
        if(!query.exec(str))
        {
            qDebug() << query.lastError();
        }
        QMessageBox::warning(this,"提示","删除成功",QMessageBox::Ok);
        dbList("combotype");
    }
}
