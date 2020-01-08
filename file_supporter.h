#ifndef FILE_SUPPORTER_H
#define FILE_SUPPORTER_H

#include "vector"
#include "file.h"
#include "disk_block.h"
#include "QByteArray"
#include "file_constant.h"
#include "string"
#include "string_utils.h"
#include "disk_constant.h"
#include "pair0.h"
#include "byte_utils.h"

using namespace std;

/**
 * 描述: 文件操作的辅助类，为文件操作提供各种解析方法
 */
class file_supporter
{
public:
    file_supporter();


    /**
     * 寻找字节数组里的空的空间
     * 此方法用于文件的创建，因为文件的创建需要修改目录信息，
     * 因此需要查找某个目录所占的那块磁盘块里的空闲空间，添加新的文件信息。
     *
     * @param bytes 字节数组
     * @param pace 步长
     * @param symbol 空闲空间的标志符
     * @return 空闲空间的开始下标，如果返回-1表示没有空闲空间
     */
    static int findEmptySpaceOfDiskBlock(QByteArray *bytes, int pace, char symbol);

    /**
     * 把磁盘块解析成文件列表
     *
     * @param diskBlock 磁盘块
     * @return List<File> 文件列表
     */
    static vector<file>* parseDiskBlockToFileList(disk_block* diskBlock);

    /**
     * 通过一个字节数组创建一个文件
     *
     * @param bytes 字节数组
     * @param offset 字节数字的读取偏移量
     * @return File 文件
     */
    static file* createFileByBytes(QByteArray* bytes, int offset);
    /**
     * 通过一个字节构造文件属性对象
     *
     * @param attribute 字节属性
     * @return FileAttribute 文件属性
     */
    static file_attribute* createFileAttributeByByte(char attribute);

    /**
     * 把一个文件解析成字节数组
     *
     * @param file 文件
     * @return byte[] 字节数组
     */
    static QByteArray* parseFileToBytes(file* file);

    /**
     * 解析文件属性成一个字节
     *
     * @param fileAttribute 文件属性
     * @return 字节
     */
    static char parseFileAttributeToByte(file_attribute* fileAttribute);
    /**
     * 判断一个文件名是否合法
     *
     * @param fileName 文件名
     * @return 是否合法
     */
    static bool legalFileName(QString fileName);

    /**
     * 判断一个目录名是否合法
     *
     * @param directoryName 目录名
     * @return 是否合法
     */
    static bool legalDirectoryName(QString directoryName);

    /**
     * 生成一个空的目录磁盘块
     * @return byte[] 空目录磁盘块
     */
    static QByteArray* getEmptyDirectoryDiskBlock();

    /**
     * 获取文件名，会自动拼接文件名和类型
     *
     * @param file 文件
     * @return 文件名
     */
    static QString getFileName(file* file);

    /**
     * 获取结束标志的下标
     *
     * @param bytes 字节数组
     * @return 结束标志的下标
     */
    static int getEndOfFileSymbolIndex(QByteArray* bytes);

    /**
     * 获得一个文件在目录磁盘块内的下标
     *
     * @param bytes 磁盘块
     * @param file 文件
     * @return 目录磁盘块内的下标
     */
    static int getDiskBlockIndexOfFile(QByteArray* bytes, file* file);

    /**
     * 把文件名解析成文件名和类型
     *
     * @param fileName 文件名
     * @return Pair<String, String>
     */
    static pair0<QString, QString>* parseFileName(QString fileName);
    /**
     * 从路径里获取文件名，包括文件类型
     *
     * @param path 路径
     * @return 文件名
     */
    static QString getFileName(QString path);

};

#endif // FILE_SUPPORTER_H
