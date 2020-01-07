#ifndef DISK_BLOCK_H
#define DISK_BLOCK_H
#include "vector"
#include "QByteArray"
using namespace std;

/**
 * 描述: 磁盘块
 */
class disk_block
{
private:
    int index;
    QByteArray *bytes;

public:
    disk_block(int index, QByteArray *bytes);
    QByteArray* getBytes();

    void setBytes(QByteArray *bytes);

    int getIndex();

    void setIndex(int index);
};

#endif // DISK_BLOCK_H
