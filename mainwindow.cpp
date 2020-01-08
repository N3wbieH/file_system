#include "mainwindow.h"
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    connect(ui->loadButton,SIGNAL(clicked()),this,SLOT(load()));

    for (int i = 0; i < 8; ++i) {
        QGridLayout* content =  ui->content;
        content->addWidget(new item_widget(this), i/4, i%4);
    }
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
    int use = static_cast<int>(0.5 * 360);
    int total = 1000;
    drawPieChart(use);

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

    static int nums = 8;
    for (int i = 0; i < nums; ++i) {
        item_widget* item = static_cast<item_widget*>(content->itemAt(i)->widget());


        item->setFile(this);
    }

    nums--;


}

void MainWindow::on_backButton_clicked()
{
    int end = curPath.lastIndexOf('/');
    if(end <= 0) {
        return;
    }

    setCurPath(curPath.left(end));
    qDebug()<<curPath;
}



void MainWindow::on_pathBar_returnPressed()
{
    if (true) {
        setCurPath(ui->pathBar->text());
    }
}


void MainWindow::saveFile(QString filePath, QString content){
    qDebug()<<content;

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
    } else {
       //创建文件夹
    }

    load();
    update();
}

void MainWindow::renameFile(QString name){


    load();
}

void MainWindow::deleteFile(QString filePath){

    load();
    update();
}


