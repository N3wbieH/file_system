#include "file_supporter.h"
#include "pair0.h"

file_supporter::file_supporter(){}

int file_supporter::findEmptySpaceOfDiskBlock(QByteArray bytes, int pace, char symbol) {
    for (int i = 0; i < static_cast<int>(bytes.size()); i += pace) {
        if (bytes[i] == symbol) {
            return i;
        }
    }
    return -1;
}
QString file_supporter::getFileName(file* file) {
    if (file->getFileAttribute()->isDirectory()) {
        QString n = file->getName();
        return n;
    }
    if (file->getType() == "") {
        return file->getName();
    }
    return file->getName() + file_constant::FILE_NAME_SEPARATOR + file->getType();
}
QString file_supporter::getFileName(QString path) {
    int index = path.lastIndexOf("/");
    if (index == -1) {
        return path;
    }
    string str = path.toStdString().substr(static_cast<unsigned long long>(index + 1));
    return QString::fromStdString(str);
}

vector<file>* file_supporter::parseDiskBlockToFileList(disk_block* diskBlock) {
    QByteArray bytes = diskBlock->getBytes();
    vector<file>* fileList = new vector<file>;
    for (int i = 0; i < (bytes.size() / file_constant::SIZE_OF_FILE); i++) {
        // 空文件
        if (bytes[i * file_constant::SIZE_OF_FILE] == file_constant::EMPTY_FILE_SYMBOL) {
            continue;
        }
        fileList->push_back(*createFileByBytes(bytes, i * file_constant::SIZE_OF_FILE));
    }
    return fileList;
}

file* file_supporter::createFileByBytes(QByteArray bytes, int offset) {
    QString name = byte_utils::bytesToString(bytes, offset, file_constant::SIZE_OF_NAME);
    QString type = byte_utils::bytesToString(bytes, offset + file_constant::SIZE_OF_NAME, file_constant::SIZE_OF_TYPE);
    file_attribute* fileAttribute = createFileAttributeByByte(bytes[offset + file_constant::SIZE_OF_NAME
            + file_constant::SIZE_OF_TYPE]);
    int firstDiskBlockIndex = bytes[offset + file_constant::SIZE_OF_NAME + file_constant::SIZE_OF_TYPE
            + file_constant::SIZE_OF_ATTRIBUTE];
    int length = bytes[offset + file_constant::SIZE_OF_NAME + file_constant::SIZE_OF_TYPE
            + file_constant::SIZE_OF_ATTRIBUTE + file_constant::SIZE_OF_FIRST_DISK_BLOCK_INDEX];
    return new file(name, type, fileAttribute, firstDiskBlockIndex, length);
}

file_attribute* file_supporter::createFileAttributeByByte(char attribute) {
    bool* booleans = byte_utils::byteToBooleans(attribute);
    bool readOnly = booleans[file_constant::SIZE_OF_FILE - file_constant::INDEX_OF_READ_ONLY - 1];
    bool system = booleans[file_constant::SIZE_OF_FILE - file_constant::INDEX_OF_SYSTEM - 1];
    bool readWrite = booleans[file_constant::SIZE_OF_FILE - file_constant::INDEX_OF_READ_WRITE - 1];
    bool directory = booleans[file_constant::SIZE_OF_FILE - file_constant::INDEX_OF_DIRECTORY - 1];
    return new file_attribute(readOnly, system, readWrite, directory);
}

QByteArray file_supporter::parseFileToBytes(file* file) {
    QByteArray bytes;
    bytes.resize(file_constant::SIZE_OF_FILE);
    int i = 0;
    // 解析文件名
    QByteArray name = file->getName().toLocal8Bit();
    for (; i < static_cast<int>(name.size()); i++) {
        bytes[i] = name[i];
    }
    for (; i < file_constant::SIZE_OF_NAME; i++) {
        bytes[i] = 0;
    }
    QByteArray type = (file->getType() == nullptr || file->getType().trimmed() == "") ? "" : file->getType().toLocal8Bit();
    int j;
    for (j = 0; j < static_cast<int>(type.length()); j++) {
        bytes[i++] = type[j];
    }
    for (; i < file_constant::SIZE_OF_NAME + file_constant::SIZE_OF_TYPE; i++) {
        bytes[i] = 0;
    }
    // 解析文件属性
    bytes[i++] = parseFileAttributeToByte(file->getFileAttribute());
    // 文件起始磁盘块下标
    bytes[i++] = static_cast<char>(file->getFirstDiskBlockIndex());
    // 文件长度
    bytes[i] = static_cast<char>(file->getLength());
    return bytes;
}


char file_supporter::parseFileAttributeToByte(file_attribute* fileAttribute) {
    int attribute = 0;
    attribute = attribute + 8 * (fileAttribute->isDirectory() ? 1 : 0);
    attribute = attribute + 4 * (fileAttribute->isReadWrite() ? 1 : 0);
    attribute = attribute + 2 * (fileAttribute->isSystem() ? 1 : 0);
    attribute = attribute + 1 * (fileAttribute->isReadOnly() ? 1 : 0);
    return static_cast<char>(attribute);
}

bool file_supporter::legalFileName(QString fileName) {
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

    QByteArray chars = fileName.toLocal8Bit();
    for (int i = 0; i < static_cast<int>(chars.length()); i++) {
        // 文件名或文件类型包含非法字符
        if (i != index && (chars[i] == '$'
                           || chars[i] == '.'
                           || chars[i] == '/')) {
            return false;
        }
    }
    return true;
}


bool file_supporter::legalDirectoryName(QString directoryName) {
    // 目录名超长
    if (directoryName.length() > file_constant::MAX_LENGTH_OF_FILE_NAME) {
        return false;
    }
    QByteArray chars = directoryName.toLocal8Bit();
    for (char c : chars) {
        // 目录名包含非法字符
        if (c == '$' || c == '.' || c == '/') {
            return false;
        }
    }
    return true;
}


QByteArray file_supporter::getEmptyDirectoryDiskBlock() {
    QByteArray bytes;
    for (int i = 0; i < disk_constant::BLOCK_SIZE; i++) {
        if (i % 8 == 0) {
            bytes[i] = file_constant::EMPTY_FILE_SYMBOL;
        } else {
            bytes[i] = 0;
        }
    }
    return bytes;
}


int file_supporter::getEndOfFileSymbolIndex(QByteArray bytes) {
    for (int i = 0; i < bytes.size(); i++) {
        if (bytes[i] == file_constant::END_OF_FILE) {
            return i;
        }
    }
    return -1;
}


int file_supporter::getDiskBlockIndexOfFile(QByteArray bytes, file* file) {
    QByteArray fileBytes = parseFileToBytes(file);
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


pair0* file_supporter::parseFileName(QString fileName) {
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
    return new pair0(name, type);
}

