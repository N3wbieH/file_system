#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QLabel>
#include <iostream>
#include "ui_mainwindow.h"
#include "txtwindow.h"
#include "item_widget.h"
#include "file_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setCurPath(QString path);
    void saveFile(QString filePath, QString content);
    void newFile(int type, QString name);
    void renameFile(QString oldName, QString newName);
    void deleteFile(QString filePath);
    QString readFile(QString filePath);

private slots:
    void load();
    void on_backButton_clicked();
    void on_pathBar_returnPressed();

private:
    Ui::MainWindow *ui;
    QMap<QString,bool> map;
    QString curPath = "/";
    file_manager *manager;

    void paintEvent(QPaintEvent *);
    void drawPieChart(int fillAngle);
    void clearContent(QLayout* layout);

};
#endif // MAINWINDOW_H
