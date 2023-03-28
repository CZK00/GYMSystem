#ifndef FACE_TRAIN_H
#define FACE_TRAIN_H

#include <math.h>
#include <iostream>
#include <fstream>
#include <QImage>
#include <sstream>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/face.hpp"
#include<opencv2\face\facerec.hpp>
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <iostream>
#include <chrono>
#include <random>
#include <ctime>
#include <QDateTime>



using namespace std;
//using namespace cv;
//using namespace cv::face;

cv::Mat norm_0_255(cv::InputArray _src);

void read_csv(const string& filename, vector<cv::Mat>& images, vector<int>& labels);

void train_xml(string fn_csv);

QImage MatImageToQt(const cv::Mat &src);

QString database_Search(int num,QSqlDatabase &db); //查询姓名

int database_FindLastnum(QSqlDatabase &db);        //查询最后编号

void write_FaceDir(QString filepath,QString user,int c);

//雪花算法
long long getTimeStamp();
void sleep_ms(unsigned int ms);
long long getNextMsTimeStamp();
long long getMachineId();

long long getSnowflaksUid();

bool compareBarData(const QDateTime &barAmount1, const QDateTime &barAmount2);


#endif // FACE_TRAIN_H
