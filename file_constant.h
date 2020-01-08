#ifndef FILE_CONSTANT_H
#define FILE_CONSTANT_H
#include "QString"
/**
 * 描述: 文件相关常量
 */
class file_constant
{
public:
    file_constant();

    /**
     * 只读标志下标
     */
    static const int INDEX_OF_READ_ONLY = 0;

    /**
     * 系统文件标志下标
     */
    static const int INDEX_OF_SYSTEM = 1;

    /**
     * 读写标志下标
     */
    static const int INDEX_OF_READ_WRITE = 2;

    /**
     * 目录标志下标
     */
    static const int INDEX_OF_DIRECTORY = 3;

    /**
     * 文件名大小
     */
    static const int SIZE_OF_NAME = 3;

    /**
     * 文件类型大小
     */
    static const int SIZE_OF_TYPE = 2;

    /**
     * 文件属性大小
     */
    static const int SIZE_OF_ATTRIBUTE = 1;

    /**
     * 文件起始磁盘块号大小
     */
    static const int SIZE_OF_FIRST_DISK_BLOCK_INDEX = 1;

    /**
     * 文件长度大小
     */
    static const int SIZE_OF_LENGTH = 1;

    /**
     * 根目录磁盘块号
     */
    static const int DISK_BLOCK_NUMBER_OF_ROOT_DIRECTORY = 2;

    /**
     * 一个文件项的大小（单位字节）
     */
    static const int SIZE_OF_FILE = 8;

    /**
     * 一个目录的最大文件数
     */
    static const int MAX_FILE_NUMBER_OF_DIRECTORY = 8;

    /**
     * 文件名与文件类型的分隔符
     */
    static const QString FILE_NAME_SEPARATOR;

    /**
     * 空文件标识符
     */
    static const char EMPTY_FILE_SYMBOL = '$';

    /**
     * 文件名最大长度
     */
    static const int MAX_LENGTH_OF_FILE_NAME = 3;

    /**
     * 文件类型最大长度
     */
    static const int MAX_LENGTH_OF_FILE_TYPE = 2;

    /**
     * 文件结束标志
     */
    static const char END_OF_FILE = -1;

    //    /**
    //     * 文件的编码方式
    //     */
    //    static const Charset ENCODING_OF_FILE = StandardCharsets.UTF_8;

};

#endif // FILE_CONSTANT_H
