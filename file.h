#ifndef FILE_H
#define FILE_H

#include "file_attribute.h"
#include "QString"
using namespace std;

/**
 * 描述: 文件
 */
class file
{
private:

    /**
     * 文件名
     */
    QString name;

    /**
     * 文件类型
     */
    QString type;

    /**
     * 文件属性
     */
    file_attribute* fileAttribute;

    /**
     * 起始盘块号
     */
    int firstDiskBlockIndex;

    /**
     * 文件长度
     */
    int length;
public:

    file(QString name, QString type, file_attribute* fileAttribute, int firstDiskBlockIndex, int length);

    file(const file &file0);

    QString getName();
    QString getType();

    file_attribute* getFileAttribute();

    int getFirstDiskBlockIndex();

    int getLength();
};

#endif // FILE_H
