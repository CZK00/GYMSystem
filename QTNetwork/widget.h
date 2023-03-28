#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <server.h>
#include <QStandardItemModel>
#include <QScrollBar>
#include <QTimer>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "face_train.h"
#include <myudpserver.h>
#include <QtCharts>
#include <QDateTimeAxis>
#include <QValueAxis>
#include "QDateTime"
#include <QtAlgorithms>
#include <myplacex.h>
#include <webserver.h>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QString na,QWidget *parent = 0);
    ~Widget();

private slots:

    //void connectionFound(const QString &IP,const int &num);

//    void connectionDestroy(const QString &IP,const int &num);

  //  void speedFeedBack(const double &speed,const int &num);

    void showClick();

    void on_BreakOFF_clicked();

    void scrollBoxBind1();       //绑定滚动条
    void scrollBoxBind2();       //绑定滚动条
    void scrollBoxBind3();       //绑定滚动条
    void scrollBoxBind4();       //绑定滚动条
    void screenRefresh();        //刷新屏幕

    //void on_db_Button_clicked();

    void menuSet();

    void on_pushButton_clicked();

    void refreshCustomerData();//更新数据

    void deleteCustomerData();//删除数据

    void searchSportData();

    void on_addNewCustom_clicked();

    void on_nextButton_clicked();

    void serviceTpyeChange();

    void readFrame();

    void searchDataType();  //数据查询子菜单

    void changeShowType();  //数据查询显示表格或者折线图

    void changeLoginType();  //数据查询显示表格或者折线图

    void on_refreshCustom_clicked();

    void on_fixCustom_clicked();

    void on_addmoneyCustom_clicked();

    void on_freezeCustom_clicked();

    void DrawLine();//绘制折线图

    void DrawRoute();//绘制折线图

    void deviceMenuSelect();

    void devicePlaceChoose(const QPointF &point);

    void deviceOnPoint(const QPointF &point, bool state);

    void systemManage();

    void changeOrderTime();

    void on_routeButton_clicked();

    void on_orderAnalysis_clicked();

    void on_orderList_clicked();

    void on_orderSearch_clicked();

    void on_searchButton2_clicked();

    void on_comboType2_currentIndexChanged(int index);

    void on_newOrderButton_clicked();

    void on_orderbutton_clicked();

    void on_deviceSearch_clicked();

    void on_newAccount_clicked();

    void on_refreshAccount_clicked();

    void on_deletAccount_clicked();

    void on_refreshOrderButton_clicked();

    void on_deleteOrderButton_clicked();

    void dirbarBack(const QModelIndex &temp);

signals:

    void change_Account();

private:
    Ui::Widget *ui;
    webserver *web;
    Server *myserver;
    MyUDPerver *udpserver;
    QTimer *updateRate;     //刷新率

    QString identification; //当前身份

    //录入信息
    int imagecount;
    bool collectstart;
    cv::CascadeClassifier face_cascade;
    vector<cv::Rect> faces;//用于保存检测结果的向量
    int face_ID;
    int surplusnum;
    QDateTime deadtime;
    QString telnum;
    QString name;
    cv::VideoCapture cap;
    cv::Mat src_image;
    QTimer *timer;//采集频率

    //DBmanager dbmanager;
    QList<QWidget*> infield;
    QList<QWidget*> menu0;
    QList<QWidget*> menu1;
    QList<QWidget*> menu2;
    QList<QWidget*> menu3;
    QList<QWidget*> menu4;
    QList<QWidget*> menu5;
    QList<QWidget*> menulast;
    QList<QWidget*> menuorder;
    QSqlDatabase db;

    //绘图
    QTimer *drawtimer;                           //计时器
    QTimer *routetimer;                           //计时器
    QChart *heartchart;                           //画布
    QSplineSeries *heartseries;                     //线
    QSplineSeries *timeseries;                     //线
    QSplineSeries *teptseries;                     //温度线
    QValueAxis *axisY;
    QValueAxis *axisY1;
    QDateTimeAxis *axisYT;
    QValueAxis *axisX;
    QHash<QDateTime,QList<float>> customerdata;
    QList<QDateTime> customerdatelist;
    int cardcount;


    int routecount;
    QLabel *label;
    QList<QLabel*> m_valueLabel;
    QChart *mapchart;
    QSplineSeries *mapseries;
    QLineSeries *trackseries;
    QValueAxis *mapX;
    QValueAxis *mapY;
    QScatterSeries *mapoint;
    QScatterSeries *mapoint2;

    QPieSeries *loginseries;

    QPieSeries *comboseries;


    void severStart();          //开始监听工作

    void menuInit();            //菜单栏初始化

    void dbList(QString str);   //表单选择

    void tableClearAll();   //清除表格内容

    void deviceTableClear();    //清除devcie表格

    void menuSelect(int num);   //选择菜单

    void setInfiedVisable(bool istrue);

    void setMenu0Visable(bool istrue);//设置菜单栏可见

    void setMenu1Visable(bool istrue);

    void setMenu2Visable(bool istrue);

    void setMenu3Visable(bool istrue);

    void setMenu4Visable(bool istrue);

    void setMenu5Visable(bool istrue);

    void setMenulastVisable(bool istrue);

    void detectFace(cv::Mat &frame);

    void addCustomerDB();

    void getfaceID(QString na);

    void heartchartInit();

};

#endif // WIDGET_H
