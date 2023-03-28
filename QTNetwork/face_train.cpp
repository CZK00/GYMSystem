#include "face_train.h"

cv::Mat norm_0_255(cv::InputArray _src)
{
    cv::Mat src = _src.getMat();
        // 创建和返回一个归一化后的图像矩阵:
        cv::Mat dst;
        switch (src.channels()) {
        case 1:
            cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
            break;
        case 3:
            cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
            break;
        default:
            src.copyTo(dst);
            break;
        }
        return dst;

}
void read_csv(const string &filename, vector<cv::Mat> &images, vector<int> &labels)
{
    std::ifstream file(filename.c_str(), ifstream::in);//c_str()函数可用可不用，无需返回一个标准C类型的字符串
        if (!file)
        {
            string error_message = "No valid input file was given, please check the given filename.";
            CV_Error(CV_StsBadArg, error_message);
        }
        string line, path, classlabel;
        while (getline(file, line)) //从文本文件中读取一行字符，未指定限定符默认限定符为“/n”
        {
            stringstream liness(line);//这里采用stringstream主要作用是做字符串的分割
            getline(liness, path,';');//读入图片文件路径以分好作为限定符
            getline(liness, classlabel);//读入图片标签，默认限定符
            if (!path.empty() && !classlabel.empty()) //如果读取成功，则将图片和对应标签压入对应容器中
            {
                images.push_back(cv::imread(path, 0));
                labels.push_back(atoi(classlabel.c_str()));
            }
        }
}
void train_xml(string fn_csv)
{
    //读取你的CSV文件路径.
        //string fn_csv = string(argv[1]);
        //fn_csv = "at.txt";

        // 2个容器来存放图像数据和对应的标签
        vector<cv::Mat> images;
        vector<int> labels;
        // 读取数据. 如果文件不合法就会出错
        // 输入的文件名已经有了.
        try
        {
            read_csv(fn_csv, images, labels); //从csv文件中批量读取训练数据
        }
        catch (cv::Exception& e)
        {
            cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
            // 文件有问题，我们啥也做不了了，退出了
            exit(1);
        }
        // 如果没有读取到足够图片，也退出.
        if (images.size() <= 1) {
            string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
            CV_Error(CV_StsError, error_message);
        }

        for (int i = 0; i < images.size(); i++)
        {
            if (images[i].size() != cv::Size(92, 112))
            {
                cout << i << endl;
                cout << images[i].size() << endl;
            }
        }


        // 下面的几行代码仅仅是从你的数据集中移除最后一张图片，作为测试图片
        //[gm:自然这里需要根据自己的需要修改，他这里简化了很多问题]
        cv::Mat testSample = images[images.size() - 1];
        int testLabel = labels[labels.size() - 1];
        images.pop_back();//删除最后一张照片，此照片作为测试图片
        labels.pop_back();//删除最有一张照片的labels
        // 下面几行创建了一个特征脸模型用于人脸识别，
        // 通过CSV文件读取的图像和标签训练它。
        // T这里是一个完整的PCA变换
        //如果你只想保留10个主成分，使用如下代码
        //      cv::createEigenFaceRecognizer(10);
        //
        // 如果你还希望使用置信度阈值来初始化，使用以下语句：
        //      cv::createEigenFaceRecognizer(10, 123.0);
        //
        // 如果你使用所有特征并且使用一个阈值，使用以下语句：
        //      cv::createEigenFaceRecognizer(0, 123.0);

        //创建一个PCA人脸分类器，暂时命名为model吧，创建完成后
        //调用其中的成员函数train()来完成分类器的训练
        cv::Ptr<cv::face::BasicFaceRecognizer> model = cv::face::EigenFaceRecognizer::create();
        model->train(images, labels);
        model->write("MyFacePCAModel.xml");//保存路径可自己设置，但注意用“\\”

        cv::Ptr<cv::face::BasicFaceRecognizer> model1 = cv::face::FisherFaceRecognizer::create();
        model1->train(images, labels);
        model1->write("MyFaceFisherModel.xml");

        cv::Ptr<cv::face::LBPHFaceRecognizer> model2 = cv::face::LBPHFaceRecognizer::create();
        model2->train(images, labels);
        model2->write("MyFaceLBPHModel.xml");

        // 下面对测试图像进行预测，predictedLabel是预测标签结果
        //注意predict()入口参数必须为单通道灰度图像，如果图像类型不符，需要先进行转换
        //predict()函数返回一个整形变量作为识别标签
        int predictedLabel = model->predict(testSample);//加载分类器
        int predictedLabel1 = model1->predict(testSample);
        int predictedLabel2 = model2->predict(testSample);

        // 还有一种调用方式，可以获取结果同时得到阈值:
        // int predictedLabel = -1;
        // double confidence = 0.0;
        //  model->predict(testSample, predictedLabel, confidence);

        string result_message = cv::format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
        string result_message1 = cv::format("Predicted class = %d / Actual class = %d.", predictedLabel1, testLabel);
        string result_message2 = cv::format("Predicted class = %d / Actual class = %d.", predictedLabel2, testLabel);
        cout << result_message << endl;
        cout << result_message1 << endl;
        cout << result_message2 << endl;



}


//Mat转成QImage
QImage MatImageToQt(const cv::Mat &src)
{
    //CV_8UC1 8位无符号的单通道---灰度图片
    if(src.type() == CV_8UC1)
    {
        //使用给定的大小和格式构造图像
        //QImage(int width, int height, Format format)
        QImage qImage(src.cols,src.rows,QImage::Format_Indexed8);
        //扩展颜色表的颜色数目
        qImage.setColorCount(256);

        //在给定的索引设置颜色
        for(int i = 0; i < 256; i ++)
        {
            //得到一个黑白图
            qImage.setColor(i,qRgb(i,i,i));
        }
        //复制输入图像,data数据段的首地址
        uchar *pSrc = src.data;
        //
        for(int row = 0; row < src.rows; row ++)
        {
            //遍历像素指针
            uchar *pDest = qImage.scanLine(row);
            //从源src所指的内存地址的起始位置开始拷贝n个
            //字节到目标dest所指的内存地址的起始位置中
            memcmp(pDest,pSrc,src.cols);
            //图像层像素地址
            pSrc += src.step;
        }
        return qImage;
    }
    //为3通道的彩色图片
    else if(src.type() == CV_8UC3)
    {
        //得到图像的的首地址
        const uchar *pSrc = (const uchar*)src.data;
        //以src构造图片
        QImage qImage(pSrc,src.cols,src.rows,src.step,QImage::Format_RGB888);
        //在不改变实际图像数据的条件下，交换红蓝通道
        return qImage.rgbSwapped();
    }
    //四通道图片，带Alpha通道的RGB彩色图像
    else if(src.type() == CV_8UC4)
    {
        const uchar *pSrc = (const uchar*)src.data;
        QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
        //返回图像的子区域作为一个新图像
        return qImage.copy();
    }
    else
    {
        return QImage();
    }
}


QString database_Search(int num, QSqlDatabase &db)
{
    QString str = QString("select *from student where num = '%1'").arg(num);
    QSqlQuery query(db);
    query.exec(str);
    QString name;
    while(query.next())
    {
        name = query.value(0).toString();
    }
    return name;
}
int database_FindLastnum(QSqlDatabase &db)
{
    int i = 0;
    QSqlQuery query(db);
    query.exec("select * from student");
    while(query.next())
    {
        i++;
    }
    return i;
}
void write_FaceDir(QString filepath, QString user, int c)
{
    QFile f("at.txt");
    f.open(QIODevice::Append);
    QTextStream txtOutput(&f);
    txtOutput<<QString("%1/faceDB/%2/%3%4").arg(filepath).arg(user).arg(QString::number(c)).arg(".jpg")<<";"<<user<<endl;
    f.close();
}


long long getTimeStamp() {
  return chrono::duration_cast<chrono::milliseconds>
  (chrono::system_clock::now().time_since_epoch()).count();
}

// sleep 1ms
void sleep_ms(unsigned int ms) {
  struct timeval time;
  time.tv_sec = ms / 1000;
  time.tv_usec = (ms / 1000) % 1000000;
  //select(0, NULL, NULL, NULL, &time);
}

long long getNextMsTimeStamp() {
  sleep_ms(1);
  return getTimeStamp();
}

long long getMachineId() {
  default_random_engine e;
  e.seed(getTimeStamp());
  return e() % 1024;
}

long long lastTimeStamp = getTimeStamp();
long long sequence = 0;
long long sequenceMask = 0b111111111111;

long long getSnowflaksUid() {
  // 一位标识符，表示正负
  long long identifier = 0;
  // 41位时间戳，69年
  long long timeStamp = getTimeStamp();
  // 十位工作机器位 1024 节点
  long long machine = getMachineId();

  if (lastTimeStamp == timeStamp) {
    // 获取当前时间戳，如果等于上次时间戳（同一毫秒内），则序列号加一；否则序列号赋值为0，从0开始
    sequence = (sequence + 1) & (sequenceMask);
    if (sequence == 0) {
      timeStamp = getNextMsTimeStamp();
    }
  } else {
    sequence = 0;
  }

  lastTimeStamp = timeStamp;
  return (timeStamp << 22) | (machine << 12) | sequence;
}

//排列判断
bool compareBarData(const QDateTime &barAmount1, const QDateTime &barAmount2)
{
    if (barAmount1 < barAmount2)
    {
        return true;
    }
    return false;
}
