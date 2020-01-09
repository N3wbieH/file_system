#include "input_dialog.h"
#include "ui_input_dialog.h"
#include "mainwindow.h"
#include <QDebug>


input_dialog::input_dialog(MainWindow* mainWindow, int type, QString filePath,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::input_dialog)
{
    this->mainWindow = mainWindow;
    this->type = type;
    this->filePath = filePath;

    ui->setupUi(this);
    ui->name->setText(filePath.right(filePath.lastIndexOf("/") + 1));
}

input_dialog::~input_dialog()
{
    delete ui;
}

void input_dialog::on_buttonBox_accepted()
{
    QString name = ui->name->text();
    if (type < 2) {
        mainWindow->newFile(type,name);
    } else {
        mainWindow->renameFile(filePath, name);
    }
    close();
}
