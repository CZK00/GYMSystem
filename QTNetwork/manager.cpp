#include "manager.h"
#include "ui_manager.h"

Manager::Manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);
    ui->VideoView->setScaledContents(true);
    ui->turnbackButton->setEnabled(false);
    ui->childWidget->setVisible(false);

    ui->listWidget->setIconSize(QSize(40,40));
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setSpacing(10);

    ui->childWidget->setIconSize(QSize(40,40));
    ui->childWidget->setResizeMode(QListView::Adjust);
    ui->childWidget->setViewMode(QListView::IconMode);
    ui->childWidget->setMovement(QListView::Static);
    ui->childWidget->setSpacing(10);

    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(listWidgetClicked(QListWidgetItem*)));
    connect(ui->childWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(childWidgetClicked(QListWidgetItem*)));
    //connect(ui->childWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(childWidgetClicked(QListWidgetItem*)));
    //setAttribute(Qt::WA_DeleteOnClose); //关闭时释放内存
    addItem("xx",QImage("xx.png"));

    ui->saveButton->setVisible(false);
}

Manager::~Manager()
{
    delete ui;
}
void Manager::closeEvent(QCloseEvent *event)

{
    Q_UNUSED(event);
    qDebug("over");
    emit shutDown();
}


void Manager::showImage(QImage img)
{
    ui->VideoView->clear();
    ui->VideoView->setPixmap(QPixmap::fromImage(img));
    update();
}

void Manager::addItem(QString name,QImage img)
{
    if(ui->listWidget->findItems(name,Qt::MatchExactly).size() == 0)
    {
        ui->listWidget->addItem(new QListWidgetItem(QIcon("xx.png"),name));
        QListWidget *childManager = new QListWidget;
        childManager->setVisible(false);
        childManager->setIconSize(QSize(40,40));
        childManager->setResizeMode(QListView::Adjust);
        childManager->setViewMode(QListView::IconMode);
        childManager->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(img)),name));
        dataManager.insert(name,childManager);
    }
    else
    {

        dataManager.value(name)->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(img)),name));
    }
}

void Manager::listWidgetClicked(QListWidgetItem *item)
{
    qDebug()<<item->text();
    ui->listWidget->setVisible(false);
    ui->childWidget->setVisible(true);
    for(int i = 0;i < dataManager.value(item->text())->count();i++)
    {
        ui->childWidget->addItem(new QListWidgetItem(dataManager.value(item->text())->item(i)->icon(),item->text()));
    }
    ui->turnbackButton->setEnabled(true);
}
void Manager::childWidgetClicked(QListWidgetItem *item)
{
    showImage(item->icon().pixmap(ui->VideoView->size()).toImage());
}

void Manager::on_deleteButton_clicked()
{

    qDebug()<<"delete";
    dataManager.take(ui->childWidget->currentItem()->text());
    ui->childWidget->takeItem(ui->childWidget->currentRow());
}

void Manager::on_turnbackButton_clicked()
{
    ui->childWidget->clear();
    ui->childWidget->setVisible(false);
    ui->listWidget->setVisible(true);
    ui->turnbackButton->setEnabled(false);
}

void Manager::on_saveButton_clicked()
{
    QDir *folder = new QDir;
    QString file_path = QFileDialog::getExistingDirectory(this,"选择目录","file");
}
