#include "txtwindow.h"
#include "ui_txtwindow.h"

txtWindow::txtWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::txtWindow)
{
    ui->setupUi(this);
}

txtWindow::~txtWindow()
{
    delete ui;
}

void txtWindow::on_save_triggered()
{

}

void txtWindow::on_close_triggered()
{
    close();
}
