#include "disk_manager.h"
#include "disk_constant.h"
#include "QFile"
#include<fstream>


disk_manager::disk_manager(file_allocation_table *fileAllocationTable) : fileAllocationTable(fileAllocationTable){
}

void disk_manager::writeDiskBlock(disk_block *diskBlock) {
    QFile file(disk_constant::DISK_NAME);
    file.open(QIODevice::WriteOnly);
    file.seek(diskBlock->getIndex() * disk_constant::BLOCK_SIZE);
    QByteArray *bytes = diskBlock -> getBytes();
    file.write(*bytes, bytes->size());
    file.close();
}

void disk_manager::writeDiskBlock(disk_block *diskBlock, int offset, int length, int diskBlockIndex, int boffset) {
    QFile file(disk_constant::DISK_NAME);
    file.open(QIODevice::WriteOnly);
    file.seek(diskBlockIndex * disk_constant::BLOCK_SIZE + boffset);
    QByteArray *bytes0 = diskBlock -> getBytes();
    file.write(byte_utils::subQByteArray(bytes0, offset, length), bytes0->size());
    file.close();
}

void disk_manager::writeDiskBlockList(vector<disk_block> diskBlockList) {
    for (vector<disk_block>::iterator it = diskBlockList.begin(); it != diskBlockList.end(); ++it) {
        writeDiskBlock(&*it);
    }
}

void disk_manager::updateFileAllocationTable() {
    for (int i = 0, j = 0; i < 2; i++) {
        QByteArray bytes;
        for (int k = 0; k < disk_constant::BLOCK_SIZE; k++, j++) {
            bytes.push_back(static_cast<char>(fileAllocationTable->getItem(j)->next));
        }
        writeDiskBlock(new disk_block(i, &bytes));
    }
}

disk_block* disk_manager::getDiskBlock(int index) {
    QFile file(disk_constant::DISK_NAME);
    file.open(QIODevice::ReadOnly);
    file.skip(index * disk_constant::BLOCK_SIZE);
    char buf[disk_constant::BLOCK_SIZE];
    file.read(buf, disk_constant::BLOCK_SIZE);
    file.close();
    QByteArray block = QByteArray::fromRawData(buf, sizeof(buf));
    return new disk_block(index, &block);
}

vector<disk_block> disk_manager::getDiskBlocksStartWith(int startIndex) {
    vector<item> itemList = fileAllocationTable->getItemsStartWith(startIndex);
    vector<disk_block> diskBlockList;
    for (auto it = itemList.begin(); it != itemList.end(); it++) {
        diskBlockList.push_back(*getDiskBlock(it->next));
    }
    return diskBlockList;
}

disk_block* disk_manager::checkAndUpdateFileAllocationTable(item *item) {
    // 已经没有空闲磁盘块
    if (item == nullptr) {
        return nullptr;
    }

    disk_block *diskBlock = getDiskBlock(item->next);

    // 分配新的磁盘块之后要更新文件分配表到磁盘
    updateFileAllocationTable();
    return diskBlock;
}

disk_block* disk_manager::allocateDiskBlock() {
    item *item0 = fileAllocationTable->allocateItem();
    return checkAndUpdateFileAllocationTable(item0);
}

disk_block* disk_manager::allocateDiskBlock(QByteArray *bytes) {
    disk_block *diskBlock = allocateDiskBlock();
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
    return checkAndUpdateFileAllocationTable(fileAllocationTable->allocateItem(previous));
}

void disk_manager::releaseDiskBlocksStartWith(int start) {
    fileAllocationTable->releaseItemsStartWith(start);
    updateFileAllocationTable();
}

void disk_manager::releaseDiskBlocksPreviousWith(int previous) {
    fileAllocationTable->releaseItemsPreviousWith(previous);
    updateFileAllocationTable();
}
