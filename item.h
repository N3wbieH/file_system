#ifndef ITEM_H
#define ITEM_H
#include "QString"
/**
* 文件分配表的项
*/
class item
{
public:
    item();
    item(int index, int next);
    QString toString();
    /**
     * 当前文件分配表项的下标
     */
    int index;
    /**
     * 指向该文件的文件分配表的下一个下标
     */
    int next;
};

#endif // ITEM_H
