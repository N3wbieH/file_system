#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "disk_manager.h"
#include "directory_tree.h"
#include "pair0.h"

class file_manager
{
private:
    /**
     * 磁盘管理器
     */
    disk_manager* diskManager;
    /**
     * 目录树
     */
    directory_tree* directoryTree;

public:


       /**
        * 文件管理器构造器
        *
        * @param diskManager 磁盘管理器
        */
       file_manager(disk_manager* diskManager) {
           this->diskManager = diskManager;
//           init();
       }

       /**
        * 创建一个目录
        *
        * @param directoryPath 文件目录路径
        * @param directoryName 目录名
        * @param system 是否是系统文件
        *
        * @return 创建的目录文件
        * @throws IOException IO操作出错
        * @throws IllegalArgumentException 非法目录名
        * @throws NotFoundException 目录不存在
        * @throws DuplicateException 文件名已经存在
        */
       file* createDirectory(QString directoryPath, QString directoryName, bool system) {
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

           for (vector<node>::iterator child = directory->getChildren()->begin();
                child != directory->getChildren()->end(); child++) {
               file* file = child->getFile();
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
           disk_block* newDiskBlock = diskManager->allocateDiskBlock(file_supporter::getEmptyDirectoryDiskBlock());
           // 初始化目录磁盘块
           return createFile(directoryPath, directoryName, nullptr,
                   new file_attribute(false, system, true, true), newDiskBlock->getIndex());
       }

       /**
        * 创建一个普通文件
        *
        * @param directoryPath 文件目录路径
        * @param fileName 文件名
        * @param system 是否是系统文件
        *
        * @return 创建的文件
        * @throws IOException IO操作出错
        * @throws IllegalArgumentException 非法文件名
        * @throws NotFoundException 目录不存在
        * @throws DuplicateException 文件名已经存在
        */
       file* createFile(QString directoryPath, QString fileName, bool system) {
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
           for (vector<node>::iterator child = directory->getChildren()->begin();
                child != directory->getChildren()->end(); child++) {
               file* file = child->getFile();
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
           pair0<QString, QString>* fileName0 = file_supporter::parseFileName(fileName);

           // 为文件分配一块磁盘块
           QByteArray newByte;
           newByte.push_back(-1);
           disk_block* newDiskBlock = diskManager->allocateDiskBlock(&newByte);
           // 创建文件
           return createFile(directoryPath, fileName0->first, fileName0->second, new file_attribute(false, system, true, false),
                   newDiskBlock->getIndex());
       }

       /**
        * 删除文件
        *
        * @param path 文件路径
        * @throws NotFoundException 文件不存在
        * @throws IOException IO操作出错
        * @throws IllegalOperationException 无法删除非空目录
        */
       void deleteFile(QString path) {
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
               diskManager->releaseDiskBlocksStartWith(file->getFirstDiskBlockIndex());
           }
           // 获取父节点的磁盘块
           disk_block* diskBlock = diskManager->getDiskBlock(node->getParent()->getFile()->getFirstDiskBlockIndex());
           // 获得此文件在目录磁盘块里的下标
           int diskBlockIndexOfFile = file_supporter::getDiskBlockIndexOfFile(diskBlock->getBytes(), file);
           // 把目录磁盘块里的此文件设置为空
           QByteArray bytes;
           bytes.push_back(file_constant::EMPTY_FILE_SYMBOL);
           diskManager->writeDiskBlock(&bytes, 0, 1, diskBlock->getIndex(), diskBlockIndexOfFile);
       }

       /**
        * 通过路径获取一个文件
        *
        * @param path 文件路径
        * @return 文件
        * @throws NotFoundException 找不到该文件
        */
       file* getFile(QString path) {
           node* node = directoryTree->getNode(path);
           // 找不到该文件
           if (node == nullptr) {
               return nullptr;
           }
           return node->getFile();
       }

       /**
        * 通过目录路径列出一个目录的所有文件
        *
        * @param directoryPath 目录路径
        * @return 该目录下的文件列表
        * @throws NotFoundException 找不到该文件夹
        * @throws IllegalArgumentException 该路径不是指向一个文件夹
        */
       vector<file>* getFileList(QString directoryPath) {
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
           for (vector<node>::iterator child = node0->getChildren()->begin();
                child != node0->getChildren()->end(); child++) {
               fileList->push_back(*child->getFile());
           }
           return fileList;
       }

       /**
        * 更新文件属性
        *
        * @param path 文件绝对路径
        * @param newFileName 新文件名
        * @return 更新之后的文件
        * @throws IOException IO操作出错
        * @throws NotFoundException 文件不存在
        */
       file* updateFile(QString path, QString newFileName) {
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
           pair0<QString, QString>* fileName = file_supporter::parseFileName(newFileName);
           // 获取更新后的文件
           file* file0 = new file(*fileName->first, *fileName->second, file1->getFileAttribute(),
                   file1->getFirstDiskBlockIndex(), file1->getLength());
           node0->setFile(file0);
           // 获取该文件目录的磁盘块
           disk_block* diskBlock = diskManager->getDiskBlock(node0->getParent()->getFile()->getFirstDiskBlockIndex());
           // 获取该磁盘块里该文件的起始下标
           int diskBlockIndexOfFile = file_supporter::getDiskBlockIndexOfFile(diskBlock->getBytes(), file1);
           // 把文件转换成字节数组
           QByteArray* bytes = file_supporter::parseFileToBytes(file0);
           // 更新文件信息到磁盘中
           diskManager->writeDiskBlock(bytes,0, file_constant::SIZE_OF_FILE, diskBlock->getIndex(), diskBlockIndexOfFile);
           return file0;
       }

       /**
        * 把内容写入文件
        *
        * @param path 文件路径
        * @param content 文件内容
        * @throws IOException IO操作出错
        */
       void writeFile(QString path, QString content) {
           writeFile(path, content.getBytes(file_constant::ENCODING_OF_FILE));
       }

       /**
        * 读取一个文件，使用UTF_8编码
        *
        * @param path 文件绝对路径
        * @return 读取的字符串
        * @throws IOException IO操作出错
        */
       QString readFile(QString path) {
           QByteArray* bytes = readFile(path, 0);
           return new String(bytes, FileConstant.ENCODING_OF_FILE);
       }

       /**
        * 读取一个文件
        *
        * @param path 文件绝对路径
        * @param length 读取长度
        * @return 读取的字节
        * @throws NotFoundException 找不到该文件
        * @throws IllegalArgumentException 该路径指向的是一个文件夹
        * @throws IOException IO操作出错
        */
       QByteArray* readFile(QString path, int length)  {
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
           vector<disk_block> diskBlockList = diskManager->getDiskBlocksStartWith(node0->getFile()->getFirstDiskBlockIndex());
           // 最后一个磁盘块的文件结束标志下标
           int endOfFileSymbolIndex = file_supporter::getEndOfFileSymbolIndex(
                   diskBlockList[diskBlockList.size() - 1].getBytes());
           // 申请文件内容的空间
           QByteArray* bytes = new QByteArray;
           for (int i = 0; i < static_cast<int>(diskBlockList.size()); i++) {
               QByteArray* bytes0 = diskBlockList[static_cast<unsigned long long>(i)].getBytes();
               // 对于最后一个磁盘块只读取到结束标志的下标处
               if (i == static_cast<int>(diskBlockList.size() - 1)) {
                   bytes->append(QByteArray::fromStdString(
                                    bytes0->toStdString()
                                    .substr(0, static_cast<unsigned long long>(endOfFileSymbolIndex))));
               } else {
                   bytes->append(*bytes0);
               }
           }

           return bytes;
       }

       /**
        * 把内容写入文件
        *
        * @param path 文件路径
        * @param bytes 文件内容字节数组
        * @throws IOException IO操作出错
        * @throws NotFoundException 找不到该文件夹
        * @throws IllegalArgumentException 该路径不是指向一个文件夹
        */
       void writeFile(QString path, QByteArray* bytes) {
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
           vector<disk_block> diskBlockList = diskManager->getDiskBlocksStartWith(node0->getFile()->getFirstDiskBlockIndex());
           QByteArray bytes0;
           bytes0.append(*bytes);
           // 设置文件结束标识符
           bytes0.append(file_constant::END_OF_FILE);
           // 需要使用的磁盘块数量
           int numberOfDiskBlocks = (int) Math.ceil((double)bytes0.length / DiskConstant.BLOCK_SIZE);
           // 把字节数组的内容写入磁盘
           for (int i = 0, lastDiskBlockIndex = diskBlockList.get(0).getIndex(); i < numberOfDiskBlocks; i++) {
               DiskBlock diskBlock;
               // 如果原本的磁盘块数量还有剩余，直接写到原本的磁盘块里
               if (diskBlockList.size() > i) {
                   diskBlock = diskBlockList.get(i);
                   lastDiskBlockIndex = diskBlock.getIndex();
               }
               // 否则申请一块磁盘块拼接在上一块磁盘块的后面
               else {
                   diskBlock = diskManager.allocateDiskBlockPreviousWith(lastDiskBlockIndex);
                   lastDiskBlockIndex = diskBlock.getIndex();
               }
               // 最后一个磁盘块，只写一部分字节
               if (i + 1 == numberOfDiskBlocks) {
                   diskManager.writeDiskBlock(bytes0, i * DiskConstant.BLOCK_SIZE,
                           bytes0.length % DiskConstant.BLOCK_SIZE, diskBlock.getIndex(), 0);
               } else {
                   diskManager.writeDiskBlock(bytes0, i * DiskConstant.BLOCK_SIZE, DiskConstant.BLOCK_SIZE,
                           diskBlock.getIndex(), 0);
               }
           }
           // 如果原来的磁盘块数量大于所需要的磁盘块数量且原来的磁盘块数量大于0，释放掉多余的磁盘块
           if (diskBlockList.size() > numberOfDiskBlocks && diskBlockList.size() > 0) {
               diskManager.releaseDiskBlocksPreviousWith(diskBlockList.get(numberOfDiskBlocks - 1).getIndex());
           }
       }

       /**
        * 创建一个文件，会为文件分配一块磁盘块，会添加到磁盘里，并更新文件树
        *
        * @param directoryPath 文件的文件夹
        * @param name 文件名
        * @param type 文件类型
        * @param fileAttribute 文件属性
        * @param firstDiskBlockIndex 第一块磁盘块的下标
        * @return File 新创建的文件
        * @throws IOException IO操作出错
        */
       file createFile(String directoryPath, String name, String type, FileAttribute fileAttribute,
                               int firstDiskBlockIndex) throws IOException {
           // 新建一个文件
           File file = new File(name, type, fileAttribute, firstDiskBlockIndex, fileAttribute.isDirectory() ? 0 : 1);

           // 添加节点
           DirectoryTree.Node parent = directoryTree.addNode(directoryPath, file);
           int parentFirstDiskBlockIndex = parent.getFile().getFirstDiskBlockIndex();
           // 获取父节点的磁盘块
           DiskBlock diskBlock = diskManager.getDiskBlock(parentFirstDiskBlockIndex);
           int emptySpaceIndex = FileSupporter.findEmptySpaceOfDiskBlock(diskBlock.getBytes(), FileConstant.SIZE_OF_FILE,
                   FileConstant.EMPTY_FILE_SYMBOL);
           // 把文件转换成字节数组
           byte[] bytes = FileSupporter.parseFileToBytes(file);
           // 更新文件信息到磁盘中
           diskManager.writeDiskBlock(bytes,0, FileConstant.SIZE_OF_FILE, parentFirstDiskBlockIndex, emptySpaceIndex);
           return file;
       }

       /**
        * 文件管理器初始化方法
        */
       private void init() throws IOException {
           // 读取根目录磁盘块
           DiskBlock rootDiskBlock = diskManager.getDiskBlock(FileConstant.DISK_BLOCK_NUMBER_OF_ROOT_DIRECTORY);
           // 解析成文件列表
           List<File> children = FileSupporter.parseDiskBlockToFileList(rootDiskBlock);
           // 新建根目录文件
           File root = new File("/", "", new FileAttribute(false, true, true, true),
                   FileConstant.DISK_BLOCK_NUMBER_OF_ROOT_DIRECTORY, children.size());
           // 设置根节点
           directoryTree = new DirectoryTree(root);

           // 从根节点递归初始化
           initDirectory(directoryTree.getRoot());
       }

       /**
        * 初始化目录，递归调用
        *
        * @param directory 目录
        * @throws IOException IO操作出错
        */
       private void initDirectory(DirectoryTree.Node directory) throws IOException {
           // 把目录的所有节点添加到目录里
           // 拿到此文件的磁盘块
           DiskBlock diskBlock = diskManager.getDiskBlock(directory.getFile().getFirstDiskBlockIndex());
           // 把此磁盘块解析成文件列表
           List<File> children = FileSupporter.parseDiskBlockToFileList(diskBlock);
           // 添加此目录下的所有子节点
           for (File child : children) {
               // 如果子节点是目录，把子节点添加到目录里，再递归调用初始化子目录
               if (child.getFileAttribute().isDirectory()) {
                   DirectoryTree.Node node = new DirectoryTree.Node(directory, new ArrayList<>(), child);
                   // 先把该子节点文件添加到目录里
                   directoryTree.addNode(directory, node);
                   // 再递归调用此方法初始化子节点
                   initDirectory(node);
               }
               // 如果子节点是普通文件，把子节点添加到目录里
               else {
                   DirectoryTree.Node node = new DirectoryTree.Node(directory, null, child);
                   // 先把该子节点文件添加到目录里
                   directoryTree.addNode(directory, node);
               }
           }
       }
};

#endif // FILE_MANAGER_H
