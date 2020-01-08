#include "input_dialog.h"
#include "ui_input_dialog.h"
#include "mainwindow.h"


input_dialog::input_dialog(MainWindow* mainWindow ,int type ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::input_dialog)
{
    this->mainWindow = mainWindow;
    this->type = type;

    ui->setupUi(this);
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
        mainWindow->renameFile(name);
    }
    close();
}
