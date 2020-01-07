QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    disk_block.cpp \
    disk_constant.cpp \
    disk_manager.cpp \
    file_allocation_table.cpp \
    file_allocation_table_constant.cpp \
    item.cpp \
    main.cpp \
    mainwindow.cpp \
    test.cpp \
    txtwindow.cpp

HEADERS += \
    disk_block.h \
    disk_constant.h \
    disk_manager.h \
    file_allocation_table.h \
    file_allocation_table_constant.h \
    item.h \
    mainwindow.h \
    test.h \
    txtwindow.h

FORMS += \
    mainwindow.ui \
    txtwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
