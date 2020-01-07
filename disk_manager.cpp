#include "disk_manager.h"
#include "disk_constant.h"
#include "QFile"
#include<fstream>


disk_manager::disk_manager(file_allocation_table *fileAllocationTable) : fileAllocationTable(fileAllocationTable){
    /**
     * 初始化磁盘管理器
     * 这里不考虑初始化失败的情况，也就是不考虑磁盘有问题的情况
     *
     * @param disk 磁盘文件
     * @throws IOException IO错误，交给上一层处理
     */
//        private void init(File disk) throws IOException {
//            try (InputStream inputStream = new FileInputStream(disk)) {
//                byte[] bytes = new byte[FileAllocationTableConstant.LENGTH];
//                inputStream.read(bytes);
//                fileAllocationTable = new FileAllocationTable(bytes);
//            }
//        }
}

void disk_manager::writeDiskBlock(disk_block *diskBlock) {
    QFile file(disk_constant::DISK_NAME);
    file.open(QIODevice::WriteOnly);
    file.seek(diskBlock->getIndex() * disk_constant::BLOCK_SIZE);
    QByteArray *bytes = diskBlock -> getBytes();
    file.write(*bytes, bytes->size());
    file.close();
}
