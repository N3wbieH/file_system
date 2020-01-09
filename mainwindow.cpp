#include "mainwindow.h"
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    manager = new file_manager();
    ui->setupUi(this);

    for (int i = 0; i < 8; ++i) {
        QGridLayout* content =  ui->content;
        content->addWidget(new item_widget(this), i/4, i%4);
    }

    load();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawPieChart(int fillAngle) {
    QPainter painter(this);
    // 去除画笔
    painter.setPen(Qt::NoPen);
    // 设置反锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect_top(570, 70, 210, 140);
    painter.setBrush(QColor(qRgb(162,166,151)));
    painter.drawEllipse(rect_top);

    // 扇形起始角度
    int startAngle = 60 *16;
    // 扇形覆盖范围
    int spanAngle = fillAngle * 16;

    // 绘制扇形
    painter.setBrush(QColor(qRgb(62,173,245)));
    painter.drawPie(rect_top, startAngle, spanAngle);
}

void MainWindow::paintEvent(QPaintEvent *) {
    //    manager->
    int use = manager->getDiskUsage() * 64;
    int total = file_allocation_table_constant::LENGTH * 64;
    drawPieChart (static_cast<int>(use * 1.0 / total * 360));

    ui->useLabel->setText(QString::number(use));
    ui->freeLabel->setText(QString::number(total - use));
}



void MainWindow::clearContent(QLayout* layout) {
    for (int i = 0; i < 8; ++i) {
        item_widget* item = static_cast<item_widget*>(layout->itemAt(i)->widget());
        item->setFile(nullptr);
        QLayout* itemLayout = item->layout();

        if (itemLayout) {
            while (itemLayout->count()) {
                QWidget *pWidget=itemLayout->itemAt(0)->widget();
                if (pWidget)
                {
                    pWidget->setParent (nullptr);

                    itemLayout->removeWidget(pWidget);
                    delete pWidget;
                }
            }
        }

    }
}

void MainWindow::load() {
    QGridLayout* content =  ui->content;

    clearContent(content);

    vector<file> files = *(manager->getFileList(curPath));
    for (int i = 0; i < files.size(); ++i) {
        qDebug()<<i;
        item_widget* item = static_cast<item_widget*>(content->itemAt(i)->widget());


        item->setFile(&(files[i]), curPath);
    }
}

void MainWindow::on_backButton_clicked()
{
    int end = curPath.lastIndexOf('/');
    if(end <= 0) {
        if (curPath.length() <= 1) {
            return;
        }
        end++;
    }

    setCurPath(curPath.left(end));
}



void MainWindow::on_pathBar_returnPressed()
{
    QString path = ui->pathBar->text();
    if (manager->getFileList(path) != nullptr) {
        qDebug()<<path;
        setCurPath(path);
    }
}


void MainWindow::saveFile(QString filePath, QString content){
    manager->writeFile(filePath, content);
    update();
}

void MainWindow::setCurPath(QString path){
    curPath = path;
    ui->pathBar->setText(curPath);
    load();
}

void MainWindow::newFile(int type, QString name){
    if (type == 0) {
        //创建文件
        manager->createFile(curPath, name, false);
    } else {
        //创建文件夹
        manager->createDirectory(curPath, name, false);
    }

    load();
    update();
}

void MainWindow::renameFile(QString filePath, QString newName){
    manager->updateFile(filePath, newName);

    load();
}

void MainWindow::deleteFile(QString filePath){
    manager->deleteFile(filePath);

    load();
    update();
}

QString MainWindow::readFile(QString filePath) {
    qDebug()<<filePath;
    return manager->readFile(filePath);
}


