/********************************************************************************
** Form generated from reading UI file 'manager.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Manager
{
public:
    QLabel *VideoView;
    QListWidget *listWidget;
    QPushButton *turnbackButton;
    QListWidget *childWidget;
    QPushButton *deleteButton;
    QPushButton *saveButton;

    void setupUi(QWidget *Manager)
    {
        if (Manager->objectName().isEmpty())
            Manager->setObjectName(QStringLiteral("Manager"));
        Manager->resize(1042, 462);
        VideoView = new QLabel(Manager);
        VideoView->setObjectName(QStringLiteral("VideoView"));
        VideoView->setGeometry(QRect(10, 10, 551, 341));
        listWidget = new QListWidget(Manager);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(570, 40, 471, 271));
        turnbackButton = new QPushButton(Manager);
        turnbackButton->setObjectName(QStringLiteral("turnbackButton"));
        turnbackButton->setGeometry(QRect(570, 10, 75, 23));
        childWidget = new QListWidget(Manager);
        childWidget->setObjectName(QStringLiteral("childWidget"));
        childWidget->setGeometry(QRect(570, 40, 471, 271));
        childWidget->setSelectionRectVisible(false);
        deleteButton = new QPushButton(Manager);
        deleteButton->setObjectName(QStringLiteral("deleteButton"));
        deleteButton->setGeometry(QRect(650, 10, 75, 23));
        saveButton = new QPushButton(Manager);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(730, 10, 75, 23));

        retranslateUi(Manager);

        QMetaObject::connectSlotsByName(Manager);
    } // setupUi

    void retranslateUi(QWidget *Manager)
    {
        Manager->setWindowTitle(QApplication::translate("Manager", "Form", nullptr));
        VideoView->setText(QApplication::translate("Manager", "TextLabel", nullptr));
        turnbackButton->setText(QApplication::translate("Manager", "\350\277\224\345\233\236\344\270\212\344\270\200\347\272\247", nullptr));
        deleteButton->setText(QApplication::translate("Manager", "\345\210\240\351\231\244", nullptr));
        saveButton->setText(QApplication::translate("Manager", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Manager: public Ui_Manager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGER_H
