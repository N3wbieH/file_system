#include "file_manager.h"
#include "iostream"
using namespace std;

file_manager::file_manager() {
    init();
}

file* file_manager::createDirectory(QString directoryPath, QString directoryName, bool system) {
    // 非法目录名
    if (!file_supporter::legalDirectoryName(directoryName)) {
        return nullptr;
    }
    node* directory = directoryTree->getNode(directoryPath);
    // 目录不存在
    if (directory == nullptr) {
        return nullptr;
    }

    // 判断文件名是否已经存在此目录下

    for (node child : *(directory->getChildren())) {
        file* file = child.getFile();
        // 如果文件是目录或者文件的类型是空白
        // 也就是只存在文件名
        if (file->getFileAttribute()->isDirectory() || string_utils::isBlank(file->getType())) {
            // 此文件名已经存在
            if (directoryName == file->getName()) {
                return nullptr;
            }
        }
    }

    // 为目录分配一块磁盘块
    disk_block* newDiskBlock = diskManager.allocateDiskBlock(file_supporter::getEmptyDirectoryDiskBlock());
    // 初始化目录磁盘块
    return createFile(directoryPath, directoryName, nullptr,
            new file_attribute(false, system, true, true), newDiskBlock->getIndex());
}

file* file_manager::createFile(QString directoryPath, QString fileName, bool system) {
    // 非法文件名
    if (!file_supporter::legalFileName(fileName)) {
        return nullptr;
    }

    node* directory = directoryTree->getNode(directoryPath);
    // 目录不存在
    if (directory == nullptr) {
        return nullptr;
    }

    // 判断文件名是否已经存在此目录下
    for (node child : *(directory->getChildren())) {
        file* file = child.getFile();
        if (file->getFileAttribute()->isDirectory()) {
            // 此文件名已经存在
            if (fileName == file->getName()) {
                return nullptr;
            }
        } else {
            // 此文件名已经存在
            if (fileName == (file->getName() + "." + file->getType())) {
                return nullptr;
            }
        }
    }

    // 解析文件名
    pair0* fileName0 = file_supporter::parseFileName(fileName);

    // 为文件分配一块磁盘块
    QByteArray newByte;
    newByte.push_back(-1);
    disk_block* newDiskBlock = diskManager.allocateDiskBlock(newByte);
    // 创建文件
    return createFile(directoryPath, fileName0->first, fileName0->second, new file_attribute(false, system, true, false),
            newDiskBlock->getIndex());
}

void file_manager::deleteFile(QString path) {
    node* node = directoryTree->getNode(path);
    // 文件不存在
    if (node == nullptr) {
        return;
    }
    file* file = node->getFile();
    // 如果文件是目录，且不是空文件，不能删除
    if (file->getFileAttribute()->isDirectory() && node->getChildren()->size() > 0) {
        return;
    }
    // 在目录树里删除文件
    directoryTree->deleteNode(path);
    // 如果是文件，先释放掉文件内容所占的磁盘块
    if (!file->getFileAttribute()->isDirectory()) {
        // 释放文件占用的磁盘块
        diskManager.releaseDiskBlocksStartWith(file->getFirstDiskBlockIndex());
    }
    // 获取父节点的磁盘块
    disk_block* diskBlock = diskManager.getDiskBlock(node->getParent()->getFile()->getFirstDiskBlockIndex());
    // 获得此文件在目录磁盘块里的下标
    int diskBlockIndexOfFile = file_supporter::getDiskBlockIndexOfFile(diskBlock->getBytes(), file);
    // 把目录磁盘块里的此文件设置为空
    QByteArray bytes;
    bytes.push_back(file_constant::EMPTY_FILE_SYMBOL);
    diskManager.writeDiskBlock(bytes, 0, 1, diskBlock->getIndex(), diskBlockIndexOfFile);
}

file* file_manager::getFile(QString path) {
    node* node = directoryTree->getNode(path);
    cout << node->getFile()->getName().toStdString() << endl;
    // 找不到该文件
    if (node == nullptr) {
        return nullptr;
    }
    return node->getFile();
}

vector<file>* file_manager::getFileList(QString directoryPath) {
    node* node0 = directoryTree->getNode(directoryPath);
    // 文件夹不存在
    if (node0 == nullptr) {
        return nullptr;
    }
    // 该路径不是指向一个文件夹
    if (!node0->getFile()->getFileAttribute()->isDirectory()) {
        return nullptr;
    }
    vector<file>* fileList = new vector<file>;
    for (file child : *fileList) {
        fileList->push_back(child);
    }
    return fileList;
}

file* file_manager::updateFile(QString path, QString newFileName) {
    node* node0 = directoryTree->getNode(path);
    // 文件不存在
    if (node0 == nullptr) {
        return nullptr;
    }
    file* file1 = node0->getFile();
    // 判断文件名是否合法
    if (file1->getFileAttribute()->isDirectory()) {
        file_supporter::legalDirectoryName(newFileName);
    } else {
        file_supporter::legalFileName(newFileName);
    }
    // 解析文件名
    pair0* fileName = file_supporter::parseFileName(newFileName);
    // 获取更新后的文件
    file* file0 = new file(fileName->first, fileName->second, file1->getFileAttribute(),
            file1->getFirstDiskBlockIndex(), file1->getLength());
    node0->setFile(file0);
    // 获取该文件目录的磁盘块
    disk_block* diskBlock = diskManager.getDiskBlock(node0->getParent()->getFile()->getFirstDiskBlockIndex());
    // 获取该磁盘块里该文件的起始下标
    int diskBlockIndexOfFile = file_supporter::getDiskBlockIndexOfFile(diskBlock->getBytes(), file1);
    // 把文件转换成字节数组
    QByteArray bytes = file_supporter::parseFileToBytes(file0);
    // 更新文件信息到磁盘中
    diskManager.writeDiskBlock(bytes,0, file_constant::SIZE_OF_FILE, diskBlock->getIndex(), diskBlockIndexOfFile);
    return file0;
}


void file_manager::writeFile(QString path, QString content) {
    QByteArray qba = content.toUtf8();
    writeFile(path, qba);
}

QString file_manager::readFile(QString path) {
    QByteArray bytes = readFile(path, 0);
    QString qs = bytes;
    return qs.toUtf8();
}

QByteArray file_manager::readFile(QString path, int length)  {
    node* node0 = directoryTree->getNode(path);
    // 找不到该文件
    if (node0 == nullptr) {
        return nullptr;
    }
    // 该路径指向的是一个文件夹
    if (node0->getFile()->getFileAttribute()->isDirectory()) {
        return nullptr;
    }

    // 获取该文件的磁盘块列表
    vector<disk_block>* diskBlockList = diskManager.getDiskBlocksStartWith(node0->getFile()->getFirstDiskBlockIndex());
    // 最后一个磁盘块的文件结束标志下标
    int endOfFileSymbolIndex = file_supporter::getEndOfFileSymbolIndex(
            (*diskBlockList)[diskBlockList->size() - 1].getBytes());
    // 申请文件内容的空间
    QByteArray bytes;
//    bytes.resize(static_cast<int>((diskBlockList->size() - 1) * disk_constant::BLOCK_SIZE +
//                     static_cast<unsigned long long>(endOfFileSymbolIndex)));
    for (int i = 0; i < static_cast<int>(diskBlockList->size()); i++) {
        QByteArray bytes0 = (*diskBlockList)[static_cast<unsigned long long>(i)].getBytes();
        // 对于最后一个磁盘块只读取到结束标志的下标处
        if (i == static_cast<int>(diskBlockList->size() - 1)) {
            bytes.append(QByteArray::fromStdString(
                             bytes0.toStdString()
                             .substr(0, static_cast<unsigned long long>(endOfFileSymbolIndex))));
        } else {
            bytes.append(*bytes0);
        }
    }

    return bytes;
}


void file_manager::writeFile(QString path, QByteArray bytes) {
    node* node0 = directoryTree->getNode(path);
    // 找不到该文件
    if (node0 == nullptr) {
        return;
    }
    // 该路径指向的是一个文件夹
    if (node0->getFile()->getFileAttribute()->isDirectory()) {
        return;
    }

    // 获取该文件的磁盘块列表
    vector<disk_block>* diskBlockList = diskManager.getDiskBlocksStartWith(node0->getFile()->getFirstDiskBlockIndex());
    QByteArray bytes0;
    bytes0.resize(bytes.size() + 1);
    for (int i = 0; i < bytes.size(); i++ ) {
        bytes0[i] = bytes[i];
    }
    // 设置文件结束标识符
    bytes0[bytes0.size() - 1] = file_constant::END_OF_FILE;
    // 需要使用的磁盘块数量
    int numberOfDiskBlocks = static_cast<int>(ceil(static_cast<double>(bytes0.size()) / disk_constant::BLOCK_SIZE));
    // 把字节数组的内容写入磁盘
    for (int i = 0, lastDiskBlockIndex = (*diskBlockList)[0].getIndex(); i < numberOfDiskBlocks; i++) {
        disk_block *diskBlock;
        // 如果原本的磁盘块数量还有剩余，直接写到原本的磁盘块里
        if (static_cast<int>(diskBlockList->size()) > i) {
            diskBlock = &(*diskBlockList)[static_cast<unsigned long long>(i)];
            lastDiskBlockIndex = diskBlock->getIndex();
        }
        // 否则申请一块磁盘块拼接在上一块磁盘块的后面
        else {
            diskBlock = diskManager.allocateDiskBlockPreviousWith(lastDiskBlockIndex);
            lastDiskBlockIndex = diskBlock->getIndex();
        }
        // 最后一个磁盘块，只写一部分字节
        if (i + 1 == numberOfDiskBlocks) {
            diskManager.writeDiskBlock(bytes0, i * disk_constant::BLOCK_SIZE,
                    bytes0.size() % disk_constant::BLOCK_SIZE, diskBlock->getIndex(), 0);
        } else {
            diskManager.writeDiskBlock(bytes0, i *  disk_constant::BLOCK_SIZE,  disk_constant::BLOCK_SIZE,
                    diskBlock->getIndex(), 0);
        }
    }
    // 如果原来的磁盘块数量大于所需要的磁盘块数量且原来的磁盘块数量大于0，释放掉多余的磁盘块
    if (static_cast<int>(diskBlockList->size()) > numberOfDiskBlocks && diskBlockList->size() > 0) {
        diskManager.releaseDiskBlocksPreviousWith((*diskBlockList)[static_cast<unsigned long long>(numberOfDiskBlocks - 1)].getIndex());
    }
}


file* file_manager::createFile(QString directoryPath, QString name, QString type, file_attribute* fileAttribute,
                        int firstDiskBlockIndex) {
    // 新建一个文件
    file* file0 = new file(name, type, fileAttribute, firstDiskBlockIndex, fileAttribute->isDirectory() ? 0 : 1);

    // 添加节点
    node* parent = directoryTree->addNode(directoryPath, file0);
    int parentFirstDiskBlockIndex = parent->getFile()->getFirstDiskBlockIndex();
    // 获取父节点的磁盘块
    disk_block* diskBlock = diskManager.getDiskBlock(parentFirstDiskBlockIndex);
    int emptySpaceIndex = file_supporter::findEmptySpaceOfDiskBlock(diskBlock->getBytes(), file_constant::SIZE_OF_FILE,
            file_constant::EMPTY_FILE_SYMBOL);
    // 把文件转换成字节数组
    QByteArray bytes = file_supporter::parseFileToBytes(file0);
    // 更新文件信息到磁盘中
    diskManager.writeDiskBlock(bytes,0, file_constant::SIZE_OF_FILE, parentFirstDiskBlockIndex, emptySpaceIndex);
    return file0;
}

void file_manager::init() {
    // 读取根目录磁盘块
    disk_block* rootDiskBlock = diskManager.getDiskBlock(file_constant::DISK_BLOCK_NUMBER_OF_ROOT_DIRECTORY);
    // 解析成文件列表
    vector<file>* children = file_supporter::parseDiskBlockToFileList(rootDiskBlock);
    // 新建根目录文件
    file* root = new file("/", "", new file_attribute(false, true, true, true),
            file_constant::DISK_BLOCK_NUMBER_OF_ROOT_DIRECTORY, static_cast<int>(children->size()));
    // 设置根节点
    directoryTree = new directory_tree(root);
    // 从根节点递归初始化
    initDirectory(directoryTree->getRoot());

    int a = 3;
    a = a + 1;

}


void file_manager::initDirectory(node* directory) {
    // 把目录的所有节点添加到目录里
    // 拿到此文件的磁盘块
    disk_block* diskBlock = diskManager.getDiskBlock(directory->getFile()->getFirstDiskBlockIndex());
    // 把此磁盘块解析成文件列表
    vector<file>* children = file_supporter::parseDiskBlockToFileList(diskBlock);
    // 添加此目录下的所有子节点
//    for (file child : *children) {
//        // 如果子节点是目录，把子节点添加到目录里，再递归调用初始化子目录
//        if (child.getFileAttribute()->isDirectory()) {
//            node* node0 = new node(directory, new vector<node>, &child);
//            cout << node0->getFile()->getName().toStdString() << "!!!!zzzzzzzz" << endl;
//            // 先把该子节点文件添加到目录里
//            directoryTree->addNode(directory, node0);
//            // 再递归调用此方法初始化子节点
//            initDirectory(node0);
//            cout << node0->getFile()->getName().toStdString() << "!!!!zzzzzzzz end" << endl;
//        }
//        // 如果子节点是普通文件，把子节点添加到目录里
//        else {
//            node* node0 = new node(directory, nullptr, &child);
//            // 先把该子节点文件添加到目录里
//            directoryTree->addNode(directory, node0);
//        }
//    }

    for (vector<file>::iterator child = children->begin(); child != children->end(); child++) {
        // 如果子节点是目录，把子节点添加到目录里，再递归调用初始化子目录
        if (child->getFileAttribute()->isDirectory()) {
            node* node0 = new node(directory, new vector<node>, &*child);
            cout << node0->getFile()->getName().toStdString() << "!!!!zzzzzzzz" << endl;
            // 先把该子节点文件添加到目录里
            directoryTree->addNode(directory, node0);
            // 再递归调用此方法初始化子节点
            initDirectory(node0);
            cout << node0->getFile()->getName().toStdString() << "!!!!zzzzzzzz end" << endl;
        }
        // 如果子节点是普通文件，把子节点添加到目录里
        else {
            node* node0 = new node(directory, nullptr, &*child);
            // 先把该子节点文件添加到目录里
            directoryTree->addNode(directory, node0);
        }
    }

}
