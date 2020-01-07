#ifndef FILE_ATTRIBUTE_H
#define FILE_ATTRIBUTE_H

/**
 * 描述: 文件属性类型
 */
class file_attribute
{

private:
    /**
     * 是否是只读文件
     */
    bool readOnly;

    /**
     * 是否是系统文件
     */
    bool system;

    /**
     * 是否是读写文件
     */
    bool readWrite;

    /**
     * 是否是目录文件
     */
    bool directory;


public:
    file_attribute(bool readOnly, bool system, bool readWrite, bool directory);

    bool isReadOnly();

    bool isSystem();

    bool isReadWrite();

    bool isDirectory();

};

#endif // FILE_ATTRIBUTE_H
