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
    static int findEmptySpaceOfDiskBlock(vector<char> *bytes, int pace, char symbol);

//    /**
//     * 把磁盘块解析成文件列表
//     *
//     * @param diskBlock 磁盘块
//     * @return List<File> 文件列表
//     */
//    static vector<file> parseDiskBlockToFileList(disk_block* diskBlock) {
//        QByteArray* bytes = diskBlock->getBytes();
//        vector<file> fileList;
//        for (int i = 0; i < bytes->size() / file_constant::SIZE_OF_FILE; i++) {
//            // 空文件
//            if (bytes->data()[i * file_constant::SIZE_OF_FILE] == file_constant::EMPTY_FILE_SYMBOL) {
//                continue;
//            }
//            fileList.push_back(createFileByBytes(bytes, i * file_constant::SIZE_OF_FILE));
//        }
//        return fileList;
//    }

//    /**
//     * 通过一个字节数组创建一个文件
//     *
//     * @param bytes 字节数组
//     * @param offset 字节数字的读取偏移量
//     * @return File 文件
//     */
//    static file* createFileByBytes(byte[] bytes, int offset) {
//        String name = ByteUtils.bytesToString(bytes, offset, FileConstant.SIZE_OF_NAME).trim();
//        String type = ByteUtils.bytesToString(bytes, offset + FileConstant.SIZE_OF_NAME, FileConstant.SIZE_OF_TYPE).trim();
//        FileAttribute fileAttribute = createFileAttributeByByte(bytes[offset + FileConstant.SIZE_OF_NAME
//                + FileConstant.SIZE_OF_TYPE]);
//        int firstDiskBlockIndex = bytes[offset + FileConstant.SIZE_OF_NAME + FileConstant.SIZE_OF_TYPE
//                + FileConstant.SIZE_OF_ATTRIBUTE];
//        int length = bytes[offset + FileConstant.SIZE_OF_NAME + FileConstant.SIZE_OF_TYPE
//                + FileConstant.SIZE_OF_ATTRIBUTE + FileConstant.SIZE_OF_FIRST_DISK_BLOCK_INDEX];
//        return new File(name, type, fileAttribute, firstDiskBlockIndex, length);
//    }

//    /**
//     * 通过一个字节构造文件属性对象
//     *
//     * @param attribute 字节属性
//     * @return FileAttribute 文件属性
//     */
//    public static FileAttribute createFileAttributeByByte(byte attribute) {
//        boolean[] booleans = ByteUtils.byteToBooleans(attribute);
//        boolean readOnly = booleans[FileConstant.SIZE_OF_FILE - FileConstant.FileAttribute.INDEX_OF_READ_ONLY - 1];
//        boolean system = booleans[FileConstant.SIZE_OF_FILE - FileConstant.FileAttribute.INDEX_OF_SYSTEM - 1];
//        boolean readWrite = booleans[FileConstant.SIZE_OF_FILE - FileConstant.FileAttribute.INDEX_OF_READ_WRITE - 1];
//        boolean directory = booleans[FileConstant.SIZE_OF_FILE - FileConstant.FileAttribute.INDEX_OF_DIRECTORY - 1];
//        return new FileAttribute(readOnly, system, readWrite, directory);
//    }

    /**
     * 把一个文件解析成字节数组
     *
     * @param file 文件
     * @return byte[] 字节数组
     */
    static QByteArray* parseFileToBytes(file* file) {
        QByteArray* bytes;
        int i = 0;
        // 解析文件名
        string name = file->getName().toStdString();
        for (; i < static_cast<int>(name.size()); i++) {
            bytes->push_back(name[static_cast<unsigned long long>(i)]);
        }
        for (; i < file_constant::SIZE_OF_NAME; i++) {
            bytes->push_back(static_cast<int>(0));
        }
        // 解析文件类型
        // 这里有两种情况，一种是目录文件，文件类型为空
        // 一种是普通文件，目录类型可能空
        string type = (file->getType() == nullptr || file->getType().trimmed() == "") ? "" : file->getType().toStdString();
        int j;
        for (j = 0; j < static_cast<int>(type.length()); j++) {
            bytes->push_back(type[static_cast<unsigned long long>(j)]);
        }
        for (; i < file_constant::SIZE_OF_NAME + file_constant::SIZE_OF_TYPE; i++) {
            bytes->push_back(static_cast<int>(0));
        }
        // 解析文件属性
        bytes->push_back(parseFileAttributeToByte(file->getFileAttribute()));
        // 文件起始磁盘块下标
        bytes->push_back(static_cast<char>(file->getFirstDiskBlockIndex()));
        // 文件长度
        bytes->push_back(static_cast<char>(file->getLength()));
        return bytes;
    }

    /**
     * 解析文件属性成一个字节
     *
     * @param fileAttribute 文件属性
     * @return 字节
     */
    static char parseFileAttributeToByte(file_attribute* fileAttribute) {
        int attribute = 0;
        attribute = attribute + 8 * (fileAttribute->isDirectory() ? 1 : 0);
        attribute = attribute + 8 * (fileAttribute->isReadWrite() ? 1 : 0);
        attribute = attribute + 8 * (fileAttribute->isSystem() ? 1 : 0);
        attribute = attribute + 8 * (fileAttribute->isReadOnly() ? 1 : 0);
        return static_cast<char>(attribute);
    }

    /**
     * 判断一个文件名是否合法
     *
     * @param fileName 文件名
     * @return 是否合法
     */
    static bool legalFileName(QString fileName) {
        // 文件名.文件类型 超长
        if (fileName.length() > file_constant::MAX_LENGTH_OF_FILE_NAME + file_constant::MAX_LENGTH_OF_FILE_TYPE + 1) {
            return false;
        }
        // 文件名不能以 “.”开始或结尾
        if (fileName.startsWith(file_constant::FILE_NAME_SEPARATOR) || fileName.endsWith(file_constant::FILE_NAME_SEPARATOR)) {
            return false;
        }

        int index = fileName.lastIndexOf(file_constant::FILE_NAME_SEPARATOR);
        // 文件名超长
        if (index == -1) {
            if (fileName.trimmed().length() > file_constant::MAX_LENGTH_OF_FILE_NAME) {
                return false;
            }
        }
        // 类型名超长或者文件名超长
        else if (fileName.length() - index - 1 > file_constant::MAX_LENGTH_OF_FILE_TYPE ||
                index > file_constant::MAX_LENGTH_OF_FILE_NAME) {
            return false;
        }

        string chars = fileName.toStdString();
        for (int i = 0; i < static_cast<int>(chars.length()); i++) {
            // 文件名或文件类型包含非法字符
            if (i != index && (chars[static_cast<unsigned long long>(i)] == '$'
                               || chars[static_cast<unsigned long long>(i)] == '.'
                               || chars[static_cast<unsigned long long>(i)] == '/')) {
                return false;
            }
        }
        return true;
    }

    /**
     * 判断一个目录名是否合法
     *
     * @param directoryName 目录名
     * @return 是否合法
     */
    static bool legalDirectoryName(QString directoryName) {
        // 目录名超长
        if (directoryName.length() > file_constant::MAX_LENGTH_OF_FILE_NAME) {
            return false;
        }
        string chars = directoryName.toStdString();
        for (char c : chars) {
            // 目录名包含非法字符
            if (c == '$' || c == '.' || c == '/') {
                return false;
            }
        }
        return true;
    }

    /**
     * 生成一个空的目录磁盘块
     * @return byte[] 空目录磁盘块
     */
    static QByteArray* getEmptyDirectoryDiskBlock() {
        QByteArray* bytes = new QByteArray;
        for (int i = 0; i < disk_constant::BLOCK_SIZE; i++) {
            if (i % 8 == 0) {
                bytes->push_back(file_constant::EMPTY_FILE_SYMBOL);
            } else {
                bytes->push_back(static_cast<int>(0));
            }
        }
        return bytes;
    }

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
    static int getEndOfFileSymbolIndex(QByteArray* bytes) {
        for (int i = 0; i < bytes->size(); i++) {
            if (bytes->data()[i] == file_constant::END_OF_FILE) {
                return i;
            }
        }
        return -1;
    }

    /**
     * 获得一个文件在目录磁盘块内的下标
     *
     * @param bytes 磁盘块
     * @param file 文件
     * @return 目录磁盘块内的下标
     */
    static int getDiskBlockIndexOfFile(QByteArray* bytes, file* file) {
        QByteArray* fileBytes = parseFileToBytes(file);
        for (int i = 0; i < disk_constant::BLOCK_SIZE / file_constant::SIZE_OF_FILE; i++) {
            int j;
            for (j = 0; j < file_constant::SIZE_OF_FILE; j++) {
                if (!(bytes[i * file_constant::SIZE_OF_FILE + j] == fileBytes[j])) {
                     break;
                }
            }
            if (j == file_constant::SIZE_OF_FILE) {
                return i * file_constant::SIZE_OF_FILE;
            }
        }
        return -1;
    }

    /**
     * 把文件名解析成文件名和类型
     *
     * @param fileName 文件名
     * @return Pair<String, String>
     */
    static pair0<QString, QString>* parseFileName(QString fileName) {
        // 寻找 “.”的下标
        int index = fileName.lastIndexOf(file_constant::FILE_NAME_SEPARATOR);
        QString name;
        QString type;
        // 没有找到 “.”，说明没有文件类型
        if (index == -1) {
            name = fileName;
            type = nullptr;
        } else {
            name = QString::fromStdString(fileName.toStdString()
                                          .substr(0, static_cast<unsigned long long>(index)));
            type = QString::fromStdString(fileName.toStdString()
                                          .substr(static_cast<unsigned long long>(index + 1)));
        }
        return new pair0<QString, QString>(&name, &type);
    }

//    /**
//     * 把文件名解析成文件名和类型
//     * 这个方法会去除文件名前面的路径，也就是比如/use/dd/zz.t会解析成zz和t
//     *
//     * @param fileName 文件名
//     * @return Pair<String, String>
//     */
//    public static Pair<String, String> parseFileName0(String fileName) {
//        int index = fileName.lastIndexOf("/");
//        if (index != -1) {
//            fileName = fileName.substring(index + 1);
//        }
//        return parseFileName(fileName);
//    }

    /**
     * 从路径里获取文件名，包括文件类型
     *
     * @param path 路径
     * @return 文件名
     */
    static QString getFileName(QString path);

};

#endif // FILE_SUPPORTER_H
