#ifndef MYPLACEX_H
#define MYPLACEX_H

#include <QWidget>
#include <QtCharts>
#include <QDateTimeAxis>
#include <QValueAxis>
#include "QDateTime"
#include <QtAlgorithms>

namespace Ui {
class MyPlaceX;
}

class MyPlaceX : public QWidget
{
    Q_OBJECT

public:
    explicit MyPlaceX(QWidget *parent = 0);
    MyPlaceX(QHash<QDateTime,QList<float>> data,QList<QDateTime> list,int pla);
    ~MyPlaceX();

private:
    Ui::MyPlaceX *ui;
    QHash<QDateTime,QList<float>> customerdata;
    QList<QDateTime> customerdatelist;
    int place;
    QLineSeries *heartbeats;
    QLineSeries *tempture;
    QChart  *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QValueAxis *axisY1;
    QChartView *chartview;
    QChartView *chartview2;


    void chartInit();


};

#endif // MYPLACEX_H
