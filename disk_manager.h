#ifndef DISK_MANAGER_H
#define DISK_MANAGER_H
#include "file_allocation_table.h"
#include "disk_block.h"
#include "disk_constant.h"
#include "QFile"
#include<fstream>
/**
 * 描述: 磁盘管理器，提供磁盘操作
 */
class disk_manager
{

private:

    /**
     * 文件分配表
     */
    file_allocation_table *fileAllocationTable;

public:
    /**
     * 构造磁盘管理器
     *
     * @param disk 磁盘文件
     * @throws IOException IO错误，交给上一层处理
     */
    disk_manager(file_allocation_table *fileAllocationTable);


    /**
     * 把磁盘块写入磁盘
     *
     * @param diskBlock 磁盘块
     * @throws IOException IO错误，交给上一层处理
     */
    void writeDiskBlock(disk_block *diskBlock);

//    /**
//     * 写一块磁盘块里的某些字节
//     *
//     * @param bytes 字节数组
//     * @param offset 字节数组内的偏移量
//     * @param length 更新长度
//     * @param diskBlockIndex 磁盘块下标
//     * @param boffset 磁盘块内偏移
//     * @throws IOException IO错误，交给上一层处理
//     */
//    void writeDiskBlock(disk_block *diskBlock, int offset, int length, int diskBlockIndex, int boffset) {
//        QFile file(disk_constant::DISK_NAME);
//        file.open(QIODevice::WriteOnly);
////        randomAccessFile.seek(diskBlockIndex * DiskConstant.BLOCK_SIZE + boffset);
////        randomAccessFile.write(bytes, offset, length);
//        file.seek(diskBlockIndex * disk_constant::BLOCK_SIZE + boffset);
//        QByteArray *bytes0 = diskBlock -> getBytes();
//        bytes0->subst
//        file.write(*bytes0, bytes0->size());
//        file.close();
//    }

//    /**
//     * 把磁盘块列表写入磁盘
//     *
//     * @param diskBlockList 磁盘块列表
//     * @throws IOException IO错误，交给上一层处理
//     */
//    public void writeDiskBlockList(List<DiskBlock> diskBlockList) throws IOException {
//        for (DiskBlock diskBlock : diskBlockList) {
//            writeDiskBlock(diskBlock);
//        }
//    }

//    /**
//     * 释放磁盘块链表，链式的从start下标开始
//     * 用于释放单块或整个文件
//     *
//     * @param start 被释放的磁盘块列表的第一块
//     */
//    public void releaseDiskBlocksStartWith(int start) throws IOException {
//        fileAllocationTable.releaseItemsStartWith(start);
//        updateFileAllocationTable();
//    }

//    /**
//     * 释放一个磁盘块链表，也就是从头到尾链式释放
//     * 用于释放有多个磁盘块，且从磁盘块链表中间开始释放的情况
//     *
//     * @param previous 被释放的磁盘块的前一项的下标
//     * @throws IOException IO错误，交给上一层处理
//     */
//    public void releaseDiskBlocksPreviousWith(int previous) throws IOException {
//        fileAllocationTable.releaseItemsPreviousWith(previous);
//        updateFileAllocationTable();
//    }

//    /**
//     * 随机分配一个磁盘块，该磁盘块为独立的一块
//     *
//     * @return DiskBlock 新分配的磁盘块
//     */
//    public DiskBlock allocateDiskBlock() throws IOException {
//        FileAllocationTable.Item item = fileAllocationTable.allocateItem();
//        return checkAndUpdateFileAllocationTable(item);
//    }

//    /**
//     * 随机分配一个磁盘块，并和上一块磁盘块链接到一起
//     *
//     * @param previous 前一个磁盘块的下标
//     * @return DiskBlock 新分配的磁盘块
//     */
//    public DiskBlock allocateDiskBlockPreviousWith(int previous) throws IOException {
//        return checkAndUpdateFileAllocationTable(fileAllocationTable.allocateItem(previous));
//    }

//    /**
//     * 随机分配一个磁盘块，该磁盘块为独立的一块
//     * 并使用字节数组初始化此磁盘块
//     *
//     * @param bytes 磁盘块的初始化数据
//     * @return DiskBlock 新分配的磁盘块
//     */
//    public DiskBlock allocateDiskBlock(byte[] bytes) throws IOException {
//        DiskBlock diskBlock = allocateDiskBlock();
//        // 无法分配到磁盘块
//        if (diskBlock == null) {
//            return null;
//        }
//        // 设置磁盘块的字节数组
//        diskBlock.setBytes(bytes);
//        // 更新磁盘块
//        writeDiskBlock(diskBlock);
//        return diskBlock;
//    }

//    /**
//     * 读取一个磁盘块
//     *
//     * @param index 磁盘块下标
//     * @return DiskBlock 磁盘块
//     * @throws IOException IO错误，交给上一层处理
//     */
//    public DiskBlock getDiskBlock(int index) throws IOException {
//        try (FileInputStream fileInputStream = new FileInputStream(DiskConstant.DISK_FILE)) {
//            fileInputStream.skip(index * DiskConstant.BLOCK_SIZE);
//            byte[] block = new byte[DiskConstant.BLOCK_SIZE];
//            fileInputStream.read(block);
//            return new DiskBlock(index, block);
//        }
//    }

//    /**
//     * 读取一个磁盘块链表
//     *
//     * @param startIndex 起始磁盘块下标
//     * @return List<DiskBlock> 磁盘块列表
//     * @throws IOException IO错误，交给上一层处理
//     */
//    public List<DiskBlock> getDiskBlocksStartWith(int startIndex) throws IOException {
//        List<FileAllocationTable.Item> itemList = fileAllocationTable.getItemsStartWith(startIndex);
//        List<DiskBlock> diskBlockList = new ArrayList<>(itemList.size());
//        for (FileAllocationTable.Item item : itemList) {
//            diskBlockList.add(getDiskBlock(item.getIndex()));
//        }
//        return diskBlockList;
//    }



//    /**
//     * 检查并更新文件分配表
//     *
//     * @param item 文件分配表项
//     * @return DiskBlock 新分配的磁盘块
//     */
//    private DiskBlock checkAndUpdateFileAllocationTable(FileAllocationTable.Item item) throws IOException {
//        // 已经没有空闲磁盘块
//        if (item == null) {
//            return null;
//        }

//        DiskBlock diskBlock = null;
//        try {
//            diskBlock = getDiskBlock(item.getIndex());
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//        // 分配新的磁盘块之后要更新文件分配表到磁盘
//        updateFileAllocationTable();
//        return diskBlock;
//    }

//    /**
//     * 更新文件分配表到磁盘
//     */
//    private void updateFileAllocationTable() throws IOException {
//        for (int i = 0, j = 0; i < 2; i++) {
//            byte[] bytes = new byte[DiskConstant.BLOCK_SIZE];
//            for (int k = 0; k < DiskConstant.BLOCK_SIZE; k++, j++) {
//                bytes[k] = (byte) fileAllocationTable.getItem(j).getNext();
//            }
//            writeDiskBlock(new DiskBlock(i, bytes));
//        }
//    }


};

#endif // DISK_MANAGER_H
