#ifndef FILE_ALLOCATION_TABLE_CONSTANT_H
#define FILE_ALLOCATION_TABLE_CONSTANT_H

/**
 * 描述: 文件分配表的常量
 */
class file_allocation_table_constant
{
public:
    file_allocation_table_constant();
    /**
     * 代表盘块损坏标志
     */
    static const char DAMAGE = -2;

    /**
     * 代表文件结尾标志
     */
    static const char END = -1;

    /**
     * 代表盘块空标志
     */
    static const char EMPTY = 0;

    /**
     * 文件分配表的所占磁盘块块数
     */
    static const int NUMBER_OF_FAT_DISK_BLOCKS = 2;

    /**
     * 文件分配表长度
     */
    static const int LENGTH = 128;
};

#endif // FILE_ALLOCATION_TABLE_CONSTANT_H
