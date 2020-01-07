#ifndef TXTWINDOW_H
#define TXTWINDOW_H

#include <QMainWindow>

namespace Ui {
class txtWindow;
}

class txtWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit txtWindow(QWidget *parent = nullptr);
    ~txtWindow();

private slots:
    void on_save_triggered();

    void on_close_triggered();

private:
    Ui::txtWindow *ui;
};

#endif // TXTWINDOW_H
