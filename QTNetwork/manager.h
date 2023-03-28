#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

namespace Ui {
class Manager;
}

class Manager : public QWidget
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = 0);
    ~Manager();

    void showImage(QImage img);

    void addItem(QString name,QImage img);

signals:
    void shutDown();

private slots:
    void listWidgetClicked(QListWidgetItem *item);

    void childWidgetClicked(QListWidgetItem *item);

    void on_deleteButton_clicked();

    void on_turnbackButton_clicked();

    void on_saveButton_clicked();

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::Manager *ui;
    QHash<QString,QListWidget*> dataManager;
};

#endif // MANAGER_H
