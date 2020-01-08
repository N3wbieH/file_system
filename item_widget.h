#ifndef ITEM_WIDGET_H
#define ITEM_WIDGET_H

#include <QtCore/QObject>
//#include <QWidget>
#include <QVBoxLayout>
#include <QMenu>


class MainWindow;


class item_widget : public QWidget {

    Q_OBJECT

public:
    item_widget(MainWindow *mainWindow);

    void setFile(void* f);

private slots:
    void dialogSlot();
    void openSlot();
    void deleteSlot();

private:
    void* f = nullptr;
    QPixmap *filePic;
    QPixmap *dirPic;
    QMenu *menu;

    MainWindow * mainWindow;

    void showMenu();
    void loadContent();
};

#endif // ITEM_WIDGET_H
