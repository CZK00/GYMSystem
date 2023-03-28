#include "myplacex.h"
#include "ui_myplacex.h"

MyPlaceX::MyPlaceX(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyPlaceX)
{
    ui->setupUi(this);
}
MyPlaceX::MyPlaceX(QHash<QDateTime, QList<float> > data, QList<QDateTime> list, int pla)
{

    this->setWindowTitle(QString("place%1打卡信息").arg(QString::number(pla)));
    customerdata = data;
    customerdatelist = list;
    place = pla;
    chartInit();

}

void MyPlaceX::chartInit()
{
    heartbeats = new QLineSeries;
    tempture = new QLineSeries;
    chart  =   new QChart();
    axisY = new QValueAxis();
    axisY1 = new QValueAxis();
    axisX = new QValueAxis();
    QTextEdit *msgEdit = new QTextEdit(this);
    msgEdit->setGeometry(50,470,900,200);
    msgEdit->setFrameStyle(QTextEdit::NoFrame);
    msgEdit->setReadOnly(true);
    msgEdit->setFont(QFont(tr("Consolas"), 12));
    msgEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    QList<float> heartemp;
    QList<float> teptemp;



    QPen penY(Qt::darkBlue,3,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin);
    QPen penY1(Qt::darkRed,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    heartbeats->setPen(penY);
    tempture->setPen(penY1);
    chart->addSeries(heartbeats);
    chart->addSeries(tempture);
    axisX->setTickCount(10);                             //设置坐标轴格数
    axisY->setTickCount(6);
    axisY1->setTickCount(6);
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

    chart->addAxis(axisX,Qt::AlignBottom);               //设置坐标轴位于chart中的位置
    chart->addAxis(axisY,Qt::AlignLeft);
    chart->addAxis(axisY1,Qt::AlignRight);
    tempture->attachAxis(axisX);
    tempture->attachAxis(axisY1);

    heartbeats->attachAxis(axisX);                           //把数据添加到坐标轴上
    heartbeats->attachAxis(axisY);
    axisY->setTitleText("心跳");
    axisY1->setTitleText("温度");
    for(int i = 0; i < customerdatelist.count(); i++)
    {
        if((int)customerdata.value(customerdatelist.at(i))[2] == place)
        {
            heartbeats->append(i, customerdata.value(customerdatelist.at(i)).at(0));
            tempture->append(i, customerdata.value(customerdatelist.at(i)).at(1));
            heartemp.append(customerdata.value(customerdatelist.at(i)).at(0));
            teptemp.append(customerdata.value(customerdatelist.at(i)).at(1));

        }
    }
    axisX->setRange(0,customerdata.count());
    chartview = new QChartView(this);
    //设置橡皮筋（放大缩小）
    //chartview->setRubberBand(QChartView::HorizontalRubberBand);
    //设置抗锯齿
    chartview->setRenderHint(QPainter::Antialiasing);
    //chartview->resize(1000,400);
    chartview->setGeometry(50,50,400,400);
    chart->legend()->hide();
    chartview->setChart(chart);


    QBarSet *heartset = new QBarSet("心率");
    QBarSet *teptset = new QBarSet("体温");

    QList<qreal> temp1;
    QList<qreal> temp2;
    temp1<<0<<0<<0<<0;
    temp2<<0<<0<<0<<0;
    for(int i = 0; i < customerdatelist.count(); i++)
    {
        if((int)customerdata.value(customerdatelist.at(i))[2] == place)
        {
            if(customerdata.value(customerdatelist.at(i)).at(0)<60)
            {
                temp1[0]++;
            }
            else if(customerdata.value(customerdatelist.at(i)).at(0)<100)
            {
                temp1[1]++;
            }
            else if(customerdata.value(customerdatelist.at(i)).at(0)<140)
            {
                temp1[2]++;
            }
            else
            {
                temp1[3]++;
            }
            if(customerdata.value(customerdatelist.at(i)).at(1)<37)
            {
                temp2[0]++;
            }
            else if(customerdata.value(customerdatelist.at(i)).at(1)<38)
            {
                temp2[1]++;
            }
            else if(customerdata.value(customerdatelist.at(i)).at(1)<39)
            {
                temp2[2]++;
            }
            else
            {
                temp2[3]++;
            }
        }
    }
    heartset->append(temp1);
    teptset->append(temp2);


    QBarSeries* barSerise = new QBarSeries();
    barSerise->append(heartset);
    barSerise->append(teptset);
    QChart *chart2 = new QChart();
    chart2->addSeries(barSerise);
    chart2->setAnimationOptions(QChart::SeriesAnimations);

    QStringList str;
    str<< "20-60(36-37)" << "60-100(37-38)" << "100-140(38-39)" << "140+(37.5-39+)" ;
    QBarCategoryAxis* axisx2 = new QBarCategoryAxis();
    axisx2->append(str);
    chart2->createDefaultAxes();
    chart2->setAxisX(axisx2);


    chartview2 = new QChartView(this);
    chartview2->setChart(chart2);
    chartview2->setRenderHint(QPainter::Antialiasing);
    chartview2->setGeometry(450,50,500,400);
    //chartview2->setChart(chart);

    qSort(heartemp.begin(),heartemp.end());
    qSort(teptemp.begin(),teptemp.end());
    float x1 = 0,x2 = 0;
    for(int i = 0; i<heartemp.count();i++)
    {
        x1 += heartemp[i];
        x2 += teptemp[i];
    }
    x1 = x1/heartemp.count();
    x2 = x2/heartemp.count();
    msgEdit->append("心率：                                 体温：");
    msgEdit->append(QString("    平均值：%1                             平均值：%2    ").arg(QString::number(x1)).arg(QString::number(x2)));
    msgEdit->append(QString("    最小值：%1                               最小值：%2    ").arg(QString::number(heartemp[0])).arg(QString::number(teptemp[0])));
    msgEdit->append(QString("    最大值：%1                               最大值：%2    ").arg(QString::number(heartemp[heartemp.count()-1])).arg(QString::number(teptemp[teptemp.count()-1])));

}

MyPlaceX::~MyPlaceX()
{
    delete ui;
}
