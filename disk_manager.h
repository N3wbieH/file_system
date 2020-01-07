#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H
#include "file_allocation_table.h"
#include "disk_block.h"
#include "disk_constant.h"
#include "QFile"
#include<fstream>
#include "byte_utils.h"
/**
 * 描述: 磁盘管理器，提供磁盘操作
 */
class disk_manager
{

private:

    /**
     * 文件分配表
     */
    file_allocation_table *fileAllocationTable;

    /**
     * 更新文件分配表到磁盘
     */
    void updateFileAllocationTable();

    /**
     * 检查并更新文件分配表
     *
     * @param item 文件分配表项
     * @return DiskBlock 新分配的磁盘块
     */
    disk_block* checkAndUpdateFileAllocationTable(item *item);


public:
    /**
     * 构造磁盘管理器
     *
     * @param disk 磁盘文件
     * @throws IOException IO错误，交给上一层处理
     */
    disk_manager(file_allocation_table *fileAllocationTable);


    /**
     * 把磁盘块写入磁盘
     *
     * @param diskBlock 磁盘块
     */
    void writeDiskBlock(disk_block *diskBlock);

    /**
     * 写一块磁盘块里的某些字节
     *
     * @param bytes 字节数组
     * @param offset 字节数组内的偏移量
     * @param length 更新长度
     * @param diskBlockIndex 磁盘块下标
     * @param boffset 磁盘块内偏移

     */
    void writeDiskBlock(disk_block *diskBlock, int offset, int length, int diskBlockIndex, int boffset);

    /**
     * 把磁盘块列表写入磁盘
     *
     * @param diskBlockList 磁盘块列表
     * @throws IOException IO错误，交给上一层处理
     */
    void writeDiskBlockList(vector<disk_block> diskBlockList);

    /**
     * 释放磁盘块链表，链式的从start下标开始
     * 用于释放单块或整个文件
     *
     * @param start 被释放的磁盘块列表的第一块
     */
    void releaseDiskBlocksStartWith(int start);

    /**
     * 释放一个磁盘块链表，也就是从头到尾链式释放
     * 用于释放有多个磁盘块，且从磁盘块链表中间开始释放的情况
     *
     * @param previous 被释放的磁盘块的前一项的下标
     * @throws IOException IO错误，交给上一层处理
     */
    void releaseDiskBlocksPreviousWith(int previous);

    /**
     * 随机分配一个磁盘块，该磁盘块为独立的一块
     *
     * @return DiskBlock 新分配的磁盘块
     */
    disk_block* allocateDiskBlock();

    /**
     * 随机分配一个磁盘块，并和上一块磁盘块链接到一起
     *
     * @param previous 前一个磁盘块的下标
     * @return DiskBlock 新分配的磁盘块
     */
    disk_block* allocateDiskBlockPreviousWith(int previous);

    /**
     * 随机分配一个磁盘块，该磁盘块为独立的一块
     * 并使用字节数组初始化此磁盘块
     *
     * @param bytes 磁盘块的初始化数据
     * @return DiskBlock 新分配的磁盘块
     */
    disk_block *allocateDiskBlock(QByteArray *bytes);

    /**
     * 读取一个磁盘块
     *
     * @param index 磁盘块下标
     * @return DiskBlock 磁盘块
     * @throws IOException IO错误，交给上一层处理
     */
    disk_block* getDiskBlock(int index);

    /**
     * 读取一个磁盘块链表
     *
     * @param startIndex 起始磁盘块下标
     * @return List<DiskBlock> 磁盘块列表
     * @throws IOException IO错误，交给上一层处理
     */
    vector<disk_block> getDiskBlocksStartWith(int startIndex);

};

#endif // DISK_MANAGER_H
