#include "mainwindow.h"
#include <QLabel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->loadButton,SIGNAL(clicked()),this,SLOT(load()));

     filePic = new QPixmap(":/image/txt");
//     filePic->scaled(80,80);
     dirPic = new QPixmap(":/image/dir");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setContent(QString name, QLayout* layout) {
    QLabel *imageLabel = new QLabel();

    imageLabel->setFixedSize(100, 100);
    imageLabel->setPixmap(*filePic);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(true);
    imageLabel->setStyleSheet("background:transparent;border:2px solid red;");

    layout->addWidget(imageLabel);
    QLabel *nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("background:transparent;border:2px solid red;");
    layout->addWidget(nameLabel);

    nameLabel->setAlignment(Qt::AlignCenter);

    layout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
}

void MainWindow::clearContent(QLayout* layout) {
    for (int i = 0; i < 8; ++i) {
        QLayout* itemLayout = layout->itemAt(i)->widget()->layout();

        if (itemLayout) {
//            itemLayout->even

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

    int nums = 8;
    for (int i = 0; i < nums; ++i) {
        QWidget* item = content->itemAt(i)->widget();
        if (!(item->layout())) {
            item->setLayout(new QVBoxLayout());
        }

        setContent(QString::number(i), item->layout());
    }

}


void MainWindow::on_pushButton_clicked()
{
    txtWindow* window = new txtWindow;
    window->show();
}

void MainWindow::on_backButton_clicked()
{

}
