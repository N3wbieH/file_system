#ifndef ITEM_WIDGET_H
#define ITEM_WIDGET_H

#include <QtCore/QObject>
//#include <QWidget>
#include <QVBoxLayout>
#include <QMenu>
#include "file.h"


class MainWindow;


class item_widget : public QWidget {

    Q_OBJECT

public:
    item_widget(MainWindow *mainWindow);

    void setFile(file* f, QString parent = nullptr);

private slots:
    void dialogSlot();
    void openSlot();
    void deleteSlot();

private:
    file* f = nullptr;
    QString parent;
    QString filePath;

    QPixmap *filePic;
    QPixmap *dirPic;
    QMenu *menu;

    MainWindow * mainWindow;

    void showMenu();
    void loadContent();
};

#endif // ITEM_WIDGET_H
