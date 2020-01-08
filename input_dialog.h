#ifndef INPUT_DIALOG_H
#define INPUT_DIALOG_H

#include <QDialog>

class MainWindow;

namespace Ui {
class input_dialog;
}

class input_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit input_dialog(MainWindow *mainWindow, int type,QWidget *parent = nullptr);
    ~input_dialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::input_dialog *ui;
    MainWindow *mainWindow;
    int type;
};

#endif // INPUT_DIALOG_H
