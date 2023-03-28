/********************************************************************************
** Form generated from reading UI file 'myplacex.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYPLACEX_H
#define UI_MYPLACEX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyPlaceX
{
public:
    QLabel *label;

    void setupUi(QWidget *MyPlaceX)
    {
        if (MyPlaceX->objectName().isEmpty())
            MyPlaceX->setObjectName(QStringLiteral("MyPlaceX"));
        MyPlaceX->resize(1000, 1000);
        label = new QLabel(MyPlaceX);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(200, 500, 411, 50));

        retranslateUi(MyPlaceX);

        QMetaObject::connectSlotsByName(MyPlaceX);
    } // setupUi

    void retranslateUi(QWidget *MyPlaceX)
    {
        MyPlaceX->setWindowTitle(QApplication::translate("MyPlaceX", "Form", nullptr));
        label->setText(QApplication::translate("MyPlaceX", "helloworld", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyPlaceX: public Ui_MyPlaceX {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYPLACEX_H
