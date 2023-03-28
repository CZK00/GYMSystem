/********************************************************************************
** Form generated from reading UI file 'entrance.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTRANCE_H
#define UI_ENTRANCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_entrance
{
public:
    QLineEdit *password;
    QLineEdit *user;
    QLabel *label_2;
    QLabel *label;
    QPushButton *logInButton;
    QPushButton *cencelButton;

    void setupUi(QWidget *entrance)
    {
        if (entrance->objectName().isEmpty())
            entrance->setObjectName(QStringLiteral("entrance"));
        entrance->resize(400, 300);
        password = new QLineEdit(entrance);
        password->setObjectName(QStringLiteral("password"));
        password->setGeometry(QRect(90, 137, 191, 20));
        user = new QLineEdit(entrance);
        user->setObjectName(QStringLiteral("user"));
        user->setGeometry(QRect(90, 60, 191, 20));
        label_2 = new QLabel(entrance);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(9, 137, 24, 16));
        label = new QLabel(entrance);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(9, 60, 36, 16));
        logInButton = new QPushButton(entrance);
        logInButton->setObjectName(QStringLiteral("logInButton"));
        logInButton->setGeometry(QRect(80, 200, 75, 23));
        cencelButton = new QPushButton(entrance);
        cencelButton->setObjectName(QStringLiteral("cencelButton"));
        cencelButton->setGeometry(QRect(180, 200, 75, 23));

        retranslateUi(entrance);

        QMetaObject::connectSlotsByName(entrance);
    } // setupUi

    void retranslateUi(QWidget *entrance)
    {
        entrance->setWindowTitle(QApplication::translate("entrance", "Form", nullptr));
        label_2->setText(QApplication::translate("entrance", "\345\257\206\347\240\201", nullptr));
        label->setText(QApplication::translate("entrance", "\347\224\250\346\210\267\345\220\215", nullptr));
        logInButton->setText(QApplication::translate("entrance", "\347\231\273\345\275\225", nullptr));
        cencelButton->setText(QApplication::translate("entrance", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class entrance: public Ui_entrance {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTRANCE_H
