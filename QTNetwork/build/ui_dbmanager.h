/********************************************************************************
** Form generated from reading UI file 'dbmanager.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBMANAGER_H
#define UI_DBMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBmanager
{
public:
    QTableWidget *tableWidget;
    QComboBox *comboBox;
    QPushButton *deleteButton;
    QLabel *label;
    QPushButton *insertButton;
    QLineEdit *startTimeEdit;
    QLineEdit *endTimeEdit;
    QPushButton *searchButton;
    QLineEdit *nameEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;

    void setupUi(QWidget *DBmanager)
    {
        if (DBmanager->objectName().isEmpty())
            DBmanager->setObjectName(QStringLiteral("DBmanager"));
        DBmanager->resize(959, 476);
        tableWidget = new QTableWidget(DBmanager);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(20, 70, 921, 391));
        comboBox = new QComboBox(DBmanager);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(100, 30, 101, 22));
        deleteButton = new QPushButton(DBmanager);
        deleteButton->setObjectName(QStringLiteral("deleteButton"));
        deleteButton->setGeometry(QRect(310, 30, 75, 23));
        label = new QLabel(DBmanager);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 30, 51, 21));
        insertButton = new QPushButton(DBmanager);
        insertButton->setObjectName(QStringLiteral("insertButton"));
        insertButton->setGeometry(QRect(220, 30, 75, 23));
        startTimeEdit = new QLineEdit(DBmanager);
        startTimeEdit->setObjectName(QStringLiteral("startTimeEdit"));
        startTimeEdit->setGeometry(QRect(430, 30, 113, 20));
        endTimeEdit = new QLineEdit(DBmanager);
        endTimeEdit->setObjectName(QStringLiteral("endTimeEdit"));
        endTimeEdit->setGeometry(QRect(570, 30, 113, 20));
        searchButton = new QPushButton(DBmanager);
        searchButton->setObjectName(QStringLiteral("searchButton"));
        searchButton->setGeometry(QRect(820, 30, 75, 23));
        nameEdit = new QLineEdit(DBmanager);
        nameEdit->setObjectName(QStringLiteral("nameEdit"));
        nameEdit->setGeometry(QRect(700, 30, 101, 21));
        label_2 = new QLabel(DBmanager);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(430, 10, 54, 12));
        label_3 = new QLabel(DBmanager);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(570, 10, 54, 12));
        label_4 = new QLabel(DBmanager);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(890, 340, 54, 12));
        label_5 = new QLabel(DBmanager);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(700, 10, 54, 12));

        retranslateUi(DBmanager);

        QMetaObject::connectSlotsByName(DBmanager);
    } // setupUi

    void retranslateUi(QWidget *DBmanager)
    {
        DBmanager->setWindowTitle(QApplication::translate("DBmanager", "Form", nullptr));
        comboBox->setItemText(0, QApplication::translate("DBmanager", "customer", nullptr));
        comboBox->setItemText(1, QApplication::translate("DBmanager", "sportdata", nullptr));

        deleteButton->setText(QApplication::translate("DBmanager", "\345\210\240\351\231\244", nullptr));
        label->setText(QApplication::translate("DBmanager", "\351\200\211\346\213\251\350\241\250\346\240\274\357\274\232", nullptr));
        insertButton->setText(QApplication::translate("DBmanager", "\346\217\222\345\205\245\344\277\241\346\201\257", nullptr));
        searchButton->setText(QApplication::translate("DBmanager", "\346\237\245\350\257\242", nullptr));
        label_2->setText(QApplication::translate("DBmanager", "\350\265\267\345\247\213\346\227\266\351\227\264", nullptr));
        label_3->setText(QApplication::translate("DBmanager", "\347\273\210\346\255\242\346\227\266\351\227\264", nullptr));
        label_4->setText(QApplication::translate("DBmanager", "\350\265\267\345\247\213\346\227\266\351\227\264", nullptr));
        label_5->setText(QApplication::translate("DBmanager", "\345\247\223\345\220\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DBmanager: public Ui_DBmanager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBMANAGER_H
