#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setFixedSize(788,345);
    w.setWindowTitle("文件管理");
    w.show();
    return a.exec();
}
