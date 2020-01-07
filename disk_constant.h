#ifndef DISK_CONSTANT_H
#define DISK_CONSTANT_H
#include "file_allocation_table.h"
#include "item.h"
#include "iostream"
using namespace std;

class disk_constant
{
public:
    disk_constant();
    static const QString DISK_NAME;

//    static const File DISK_FILE = new File(System.getProperty("user.dir"), DISK_NAME);
    static const QString DISK_FILE_NAME;

    static const int BLOCK_SIZE = 64;
};
    const QString disk_constant::DISK_NAME = "system.disk";
    const QString disk_constant::DISK_FILE_NAME = "user.dir";
#endif // DISK_CONSTANT_H
