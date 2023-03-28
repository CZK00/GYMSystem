#include "entrance.h"
#include <QApplication>
#include "face_train.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    entrance w;
    w.show();
    return a.exec();
}
