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

//    /**
//     * 把一个文件解析成字节数组
//     *
//     * @param file 文件
//     * @return byte[] 字节数组
//     */
//    public static byte[] parseFileToBytes(File file) {
//        byte[] bytes = new byte[FileConstant.SIZE_OF_FILE];
//        int i = 0;
//        // 解析文件名
//        byte[] name = file.getName().getBytes();
//        for (; i < name.length; i++) {
//            bytes[i] = name[i];
//        }
//        for (; i < FileConstant.SIZE_OF_NAME; i++) {
//            bytes[i] = 0;
//        }
//        // 解析文件类型
//        // 这里有两种情况，一种是目录文件，文件类型为空
//        // 一种是普通文件，目录类型可能空
//        byte[] type = file.getType() == null || "".equals(file.getType().trim()) ? new byte[0] : file.getType().getBytes();
//        int j;
//        for (j = 0; j < type.length; j++) {
//            bytes[i++] = type[j];
//        }
//        for (; i < FileConstant.SIZE_OF_NAME + FileConstant.SIZE_OF_TYPE; i++) {
//            bytes[i] = 0;
//        }
//        // 解析文件属性
//        bytes[i++] = parseFileAttributeToByte(file.getFileAttribute());
//        // 文件起始磁盘块下标
//        bytes[i++] = (byte) file.getFirstDiskBlockIndex();
//        // 文件长度
//        bytes[i] = (byte) file.getLength();
//        return bytes;
//    }

//    /**
//     * 解析文件属性成一个字节
//     *
//     * @param fileAttribute 文件属性
//     * @return 字节
//     */
//    public static byte parseFileAttributeToByte(FileAttribute fileAttribute) {
//        String attribute = "0000";
//        attribute = attribute + (fileAttribute.isDirectory() ? "1" : "0");
//        attribute = attribute + (fileAttribute.isReadWrite() ? "1" : "0");
//        attribute = attribute + (fileAttribute.isSystem() ? "1" : "0");
//        attribute = attribute + (fileAttribute.isReadOnly() ? "1" : "0");
//        return Byte.valueOf(attribute,2);
//    }

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
                bytes->push_back(static_cast<char>(0));
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

//    /**
//     * 获取结束标志的下标
//     *
//     * @param bytes 字节数组
//     * @return 结束标志的下标
//     */
//    public static int getEndOfFileSymbolIndex(byte[] bytes) {
//        for (int i = 0; i < bytes.length; i++) {
//            if (bytes[i] == FileConstant.END_OF_FILE) {
//                return i;
//            }
//        }
//        return -1;
//    }

//    /**
//     * 获得一个文件在目录磁盘块内的下标
//     *
//     * @param bytes 磁盘块
//     * @param file 文件
//     * @return 目录磁盘块内的下标
//     */
//    public static int getDiskBlockIndexOfFile(byte[] bytes, File file) {
//        byte[] fileBytes = parseFileToBytes(file);
//        for (int i = 0; i < DiskConstant.BLOCK_SIZE / FileConstant.SIZE_OF_FILE; i++) {
//            int j;
//            for (j = 0; j < FileConstant.SIZE_OF_FILE; j++) {
//                if (!(bytes[i * FileConstant.SIZE_OF_FILE + j] == fileBytes[j])) {
//                     break;
//                }
//            }
//            if (j == FileConstant.SIZE_OF_FILE) {
//                return i * FileConstant.SIZE_OF_FILE;
//            }
//        }
//        return -1;
//    }

//    /**
//     * 把文件名解析成文件名和类型
//     *
//     * @param fileName 文件名
//     * @return Pair<String, String>
//     */
//    public static Pair<String, String> parseFileName(String fileName) {
//        // 寻找 “.”的下标
//        int index = fileName.lastIndexOf(FileConstant.FILE_NAME_SEPARATOR);
//        String name;
//        String type;
//        // 没有找到 “.”，说明没有文件类型
//        if (index == -1) {
//            name = fileName;
//            type = null;
//        } else {
//            name = fileName.substring(0, index);
//            type = fileName.substring(index + 1);
//        }
//        return new Pair<>(name, type);
//    }

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
