#include "txtwindow.h"
#include "mainwindow.h"
#include "ui_txtwindow.h"

txtWindow::txtWindow(MainWindow *mainWindow, QString filePath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::txtWindow)
{
    this->mainWindow = mainWindow;
    this->filePath = filePath;

    QString content = mainWindow->readFile(filePath);
    ui->setupUi(this);
    ui->content->setPlainText(content);
}

txtWindow::~txtWindow()
{
    delete ui;
}

void txtWindow::on_save_triggered()
{
    mainWindow->saveFile(filePath, ui->content->toPlainText());
}

void txtWindow::on_close_triggered()
{
    close();
}
