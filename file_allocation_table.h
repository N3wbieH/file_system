#ifndef FILE_ALLOCATION_TABLE_H
#define FILE_ALLOCATION_TABLE_H
#include "item.h"
#include "file_allocation_table_constant.h"
#include "vector"
using namespace std;

/**
 * 描述: 文件分配表
 */
class file_allocation_table
{
private:
    /**
     * 文件分配表项数组
     */
    vector<item>* items;

    /**
     * 初始化文件分配表
     * @param items 文件分配表的字节数组
     */
    void init();

    /**
     * 随机获取一个空文件分配表项
     *
     * 如果已经没有空闲磁盘块会返回null
     * @return Item 文件分配表项
     */
    item* getEmptyItem();

    /**
     * 更新分配表的一项
     * @param item 分配表项
     */
    void updateItem(item* item);

    /**
     * 释放文件分配表项，从一个链中摘除某一项
     *
     * @param previous 被释放的文件分配表项的前一项
     */
    void releaseItem(int previous);

public:

    /**
     * 初始化文件分配表，包括初始化保留项
     * @param items 文件分配表的字节数组
     */
    file_allocation_table();

    /**
     * 获取以startIndex为起点的文件分配表项列表
     *
     * @param startIndex 文件分配表起点下标
     * @return startIndex为起点的文件分配表项列表
     */
    vector<item>* getItemsStartWith(int startIndex);

    /**
     * 获取一个文件分配表项
     * @param index 文件分配表项下标
     * @return 文件分配表项
     */
    item* getItem(int index);

    /**
     * 随机分配一个空文件分配表项
     * 该文件分配项会作为最后一个项，也就是value==FileAllocationTableConstant.END
     * 并更新文件分配表的内容
     *
     * 如果已经没有空闲磁盘块会返回null
     *
     * @return Item 文件分配表项
     */
    item* allocateItem();

    /**
     * 随机获取一个空文件分配表项
     * 该文件分配项会插入已经分配的文件分配表项链之中
     *
     * 如果已经没有空闲磁盘块会返回null
     *
     * @param previous 前一个分配表下标
     * @return Item 新分配的文件分配表项
     */
    item* allocateItem(int previous);

    /**
     * 释放文件分配表项，链式的从start下标开始
     * 用于释放单项或整个文件
     *
     * @param start 被释放的文件分配表项的第一项
     */
    void releaseItemsStartWith(int start);

    /**
     * 释放文件分配表项，链式的从previous下标的下一项开始
     * 用于修改文件内容时使用
     *
     * @param previous 被释放的文件分配表项的前一项
     */
    void releaseItemsPreviousWith(int previous);

    /**
     * 返回磁盘占用块数
     */
    int getDiskUsage();

};

#endif // FILE_ALLOCATION_TABLE_H
