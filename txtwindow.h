#ifndef TXTWINDOW_H
#define TXTWINDOW_H

#include <QMainWindow>

class MainWindow;

namespace Ui {
class txtWindow;
}

class txtWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit txtWindow(MainWindow *mainWindow, QString filePath, QString content, QWidget *parent = nullptr);
    ~txtWindow();

private slots:
    void on_save_triggered();

    void on_close_triggered();

private:
    Ui::txtWindow *ui;
    MainWindow* mainWindow;
    QString filePath;
};

#endif // TXTWINDOW_H
