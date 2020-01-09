#include "disk_manager.h"
#include "disk_constant.h"
#include "QFile"
#include<fstream>
#include "iostream"
#include "file_constant.h"
#include "QDebug"
using namespace std;

disk_manager::disk_manager() {}

void disk_manager::writeDiskBlock(disk_block *diskBlock) {
    QFile file0(disk_constant::DISK_NAME);
    file0.open(QIODevice::ReadOnly);
    QByteArray buf = file0.read(8192);
    file0.close();

    QFile file(disk_constant::DISK_NAME);
    file.open(QIODevice::WriteOnly);
    QByteArray bytes = diskBlock ->getBytes();
    qDebug() << "-----" << bytes.size();
    for (int i = diskBlock->getIndex() * disk_constant::BLOCK_SIZE, j = 0; j < bytes.size(); i++, j++) {
        buf[i] = bytes[j];
    }
    file.write(buf, 8192);
    file.close();
}

void disk_manager::writeDiskBlock(QByteArray bytes0, int offset, int length, int diskBlockIndex, int boffset) {
    QFile file0(disk_constant::DISK_NAME);
    file0.open(QIODevice::ReadOnly);
    QByteArray buf = file0.read(8192);
    file0.close();

    QFile file(disk_constant::DISK_NAME);
    file.open(QIODevice::WriteOnly);
    for (int i = diskBlockIndex * disk_constant::BLOCK_SIZE + boffset, j = offset; j < length; i++, j++) {

        buf[i] = bytes0[j];
        qDebug() << buf[i] << " " << bytes0[j];
    }
    file.write(buf, 8192);
    file.close();
}

void disk_manager::writeDiskBlockList(vector<disk_block>* diskBlockList) {
    for (vector<disk_block>::iterator diskBlock = diskBlockList->begin(); diskBlock != diskBlockList->end(); diskBlock++) {
        writeDiskBlock(&*diskBlock);
    }
}

void disk_manager::updateFileAllocationTable() {
    for (int i = 0, j = 0; i < 2; i++) {
        QByteArray* bytes = new QByteArray;
        for (int k = 0; k < disk_constant::BLOCK_SIZE; k++, j++) {
            bytes->push_back(static_cast<char>(fileAllocationTable.getItem(j)->next));
        }
        writeDiskBlock(new disk_block(i, *bytes));
    }
}

disk_block* disk_manager::getDiskBlock(int index) {
    QFile file(disk_constant::DISK_NAME);
    file.open(QIODevice::ReadOnly);
    file.skip(index * disk_constant::BLOCK_SIZE);
//    char* buf = new char[disk_constant::BLOCK_SIZE];
//    file.read(buf, disk_constant::BLOCK_SIZE);
    QByteArray buf = file.read(disk_constant::BLOCK_SIZE);
    file.close();
//    QByteArray* block = new QByteArray(buf, disk_constant::BLOCK_SIZE);
//    delete[] buf;
    return new disk_block(index, buf);
}

vector<disk_block>* disk_manager::getDiskBlocksStartWith(int startIndex) {
    vector<item>* itemList = fileAllocationTable.getItemsStartWith(startIndex);
    vector<disk_block>* diskBlockList = new vector<disk_block>;
    for (vector<item>::iterator it = itemList->begin(); it != itemList->end(); it++) {
        diskBlockList->push_back(*getDiskBlock(it->index));
    }

    return diskBlockList;
}

disk_block* disk_manager::checkAndUpdateFileAllocationTable(item *item) {
    // 已经没有空闲磁盘块
    if (item == nullptr) {
        return nullptr;
    }

    disk_block *diskBlock = getDiskBlock(item->index);

    // 分配新的磁盘块之后要更新文件分配表到磁盘
    updateFileAllocationTable();
    return diskBlock;
}

disk_block* disk_manager::allocateDiskBlock() {
    item *item0 = fileAllocationTable.allocateItem();
    qDebug() << item0->next << "zzzzzzz";
    return checkAndUpdateFileAllocationTable(item0);
}

disk_block* disk_manager::allocateDiskBlock(QByteArray bytes) {
    disk_block *diskBlock = allocateDiskBlock();
    qDebug() << diskBlock->getIndex();
    // 无法分配到磁盘块
    if (diskBlock == nullptr) {
        return nullptr;
    }
    // 设置磁盘块的字节数组
    diskBlock->setBytes(bytes);
    // 更新磁盘块
    writeDiskBlock(diskBlock);
    return diskBlock;
}

disk_block* disk_manager::allocateDiskBlockPreviousWith(int previous) {
    return checkAndUpdateFileAllocationTable(fileAllocationTable.allocateItem(previous));
}

void disk_manager::releaseDiskBlocksStartWith(int start) {
    fileAllocationTable.releaseItemsStartWith(start);
    updateFileAllocationTable();
}

void disk_manager::releaseDiskBlocksPreviousWith(int previous) {
    fileAllocationTable.releaseItemsPreviousWith(previous);
    updateFileAllocationTable();
}

int disk_manager::getDiskUsage() {
    return fileAllocationTable.getDiskUsage();
}
