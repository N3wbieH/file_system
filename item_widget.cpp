#include "item_widget.h"
#include <QLabel>
#include "input_dialog.h"
#include "mainwindow.h"
#include "txtwindow.h"


item_widget :: item_widget(MainWindow *mainWindow) {
    this->mainWindow = mainWindow;

    this->setLayout(new QVBoxLayout());
    this->layout()->setSpacing(0);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,&QWidget::customContextMenuRequested,[=](const QPoint &pos)
    {
        if (f != nullptr) {
            this->setStyleSheet("background-color:#A4F4FF;");
        }
        showMenu();
        this->setStyleSheet("background-color:transparent;");
    });

    filePic = new QPixmap(":/image/txt");
    dirPic = new QPixmap(":/image/dir");
}

void item_widget :: setFile(file * f, QString parent) {
    this->f = f;
    this->parent = parent;

    if (f != nullptr) {
        loadContent();
    }
}

void item_widget::loadContent() {
    filePath = parent;
    if (parent.length() > 1) {
        filePath += "/";
    }
    filePath += f->getName();

    QString type = f->getType();
    if(type != nullptr && type.length() > 0) {
        filePath +=  "." + f->getType();
    }

    QLayout *layout = this->layout();

    QLabel *imageLabel = new QLabel();
    imageLabel->setFixedSize(100, 100);
    if (f->getFileAttribute()->isDirectory()) {
        imageLabel->setPixmap(*dirPic);
    } else {
        imageLabel->setPixmap(*filePic);
    }


    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(true);
    //    imageLabel->setStyleSheet("background:transparent;border:2px solid red;");

    layout->addWidget(imageLabel);
    QLabel *nameLabel = new QLabel(f->getName());
    //    nameLabel->setStyleSheet("background:transparent;border:2px solid red;");
    layout->addWidget(nameLabel);

    nameLabel->setAlignment(Qt::AlignCenter);
    QFont font ("Microsoft YaHei", 12, 50);
    nameLabel->setFont(font);

    layout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
}

void item_widget::showMenu() {
    menu = new QMenu();

    if (f == nullptr) {
        QAction *newFile = new QAction("创建文件",this);
        QAction *newDir = new QAction("创建文件夹",this);

        newFile->setData(0);
        newDir->setData(1);

        connect(newFile,SIGNAL(triggered()),SLOT(dialogSlot()));
        connect(newDir,SIGNAL(triggered()),SLOT(dialogSlot()));

        menu->addAction(newDir);
        menu->addAction(newFile);
    } else {
        QAction *open = new QAction("打开", this);
        QAction *rename = new QAction("重命名",this);
        QAction *del = new QAction("删除",this);

        rename->setData(2);

        connect(open,SIGNAL(triggered()),SLOT(openSlot()));
        connect(rename,SIGNAL(triggered()),SLOT(dialogSlot()));
        connect(del,SIGNAL(triggered()),SLOT(deleteSlot()));

        menu->addAction(open);
        menu->addAction(rename);
        menu->addAction(del);
    }

    menu->exec(QCursor::pos());

    QList<QAction*> list = menu->actions();
    foreach (QAction* pAction, list) delete pAction;
    delete menu;
}

void item_widget::dialogSlot(){
    QAction *pEvent = qobject_cast<QAction *>(this->sender());
    int type = pEvent->data().toInt();

    input_dialog* dialog;
    if(type < 2) {
        dialog = new input_dialog(mainWindow, type);
        dialog->setWindowTitle("新建");
    } else {
        dialog = new input_dialog(mainWindow, type, filePath);
        dialog->setWindowTitle("重命名");
    }
    dialog->exec();
}

void item_widget::openSlot(){
    if (f->getFileAttribute()->isDirectory()){
        mainWindow->setCurPath(filePath);
    } else {
        txtWindow* window = new txtWindow(mainWindow, filePath);
        window->setWindowTitle(f->getName());
        window->show();
    }
}

void item_widget::deleteSlot(){
    mainWindow->deleteFile(filePath);
}
