#ifndef DISK_CONSTANT_H
#define DISK_CONSTANT_H
#include "QString"

/**
 * 描述: 磁盘相关常量
 */
class disk_constant
{
public:
    disk_constant();
    static const QString DISK_NAME;

//    static const File DISK_FILE = new File(System.getProperty("user.dir"), DISK_NAME);

    static const int BLOCK_SIZE = 64;
};


#endif // DISK_CONSTANT_H
