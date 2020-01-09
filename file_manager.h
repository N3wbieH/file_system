#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "disk_manager.h"
#include "directory_tree.h"
#include "pair0.h"
#include "math.h"

class file_manager
{
    private:
    /**
     * 磁盘管理器
     */
    disk_manager diskManager;
    /**
     * 目录树
     */
    directory_tree* directoryTree;

    /**
     * 文件管理器初始化方法
     */
    void init();

    /**
     * 初始化目录，递归调用
     *
     * @param directory 目录
     */
    void initDirectory(node* directory);


    /**
     * 读取一个文件
     *
     * @param path 文件绝对路径
     * @param length 读取长度
     * @return 读取的字节
     */
    QByteArray readFile(QString path, int length);


    /**
     * 把内容写入文件
     *
     * @param path 文件路径
     * @param bytes 文件内容字节数组
     */
    void writeFile(QString path, QByteArray bytes);

    /**
     * 创建一个文件，会为文件分配一块磁盘块，会添加到磁盘里，并更新文件树
     *
     * @param directoryPath 文件的文件夹
     * @param name 文件名
     * @param type 文件类型
     * @param fileAttribute 文件属性
     * @param firstDiskBlockIndex 第一块磁盘块的下标
     * @return File 新创建的文件
     */
    file* createFile(QString directoryPath, QString name, QString type, file_attribute* fileAttribute,
                            int firstDiskBlockIndex);

public:


       /**
        * 文件管理器构造器
        */
       file_manager();

       /**
        * 创建一个目录
        *
        * @param directoryPath 文件目录路径
        * @param directoryName 目录名
        * @param system 是否是系统文件
        *
        * @return 创建的目录文件
        */
       file* createDirectory(QString directoryPath, QString directoryName, bool system);

       /**
        * 创建一个普通文件
        *
        * @param directoryPath 文件目录路径
        * @param fileName 文件名
        * @param system 是否是系统文件
        *
        * @return 创建的文件
        */
       file* createFile(QString directoryPath, QString fileName, bool system);

       /**
        * 删除文件
        *
        * @param path 文件路径
        */
       void deleteFile(QString path);

       /**
        * 通过路径获取一个文件
        *
        * @param path 文件路径
        * @return 文件
        */
       file* getFile(QString path);

       /**
        * 通过目录路径列出一个目录的所有文件
        *
        * @param directoryPath 目录路径
        * @return 该目录下的文件列表
        */
       vector<file>* getFileList(QString directoryPath);

       /**
        * 更新文件属性
        *
        * @param path 文件绝对路径
        * @param newFileName 新文件名
        * @return 更新之后的文件
        */
       file* updateFile(QString path, QString newFileName);

       /**
        * 把内容写入文件
        *
        * @param path 文件路径
        * @param content 文件内容
        */
       void writeFile(QString path, QString content);

       /**
        * 读取一个文件，使用UTF_8编码
        *
        * @param path 文件绝对路径
        * @return 读取的字符串
        */
       QString readFile(QString path);

       /**
        * 返回磁盘占用块数
        */
       int getDiskUsage();


};

#endif // FILE_MANAGER_H
