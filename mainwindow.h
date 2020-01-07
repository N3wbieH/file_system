#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "txtwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPixmap *filePic;
    QPixmap *dirPic;
private slots:
    void load();
    void on_pushButton_clicked();

    void on_backButton_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString,bool> map;

    void setContent(QString name, QLayout* layout);
    void clearContent(QLayout* layout);

};
#endif // MAINWINDOW_H
