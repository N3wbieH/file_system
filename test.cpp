#include "test.h"
#include "disk_block.h"
#include "item.h"
#include "iostream"
#include "file_allocation_table.h"
#include "vector"
#include "string"
#include "qfile.h"
#include "disk_constant.h"
#include "disk_block.h"
#include "file_allocation_table.h"
#include "disk_manager.h"
using namespace std;

void testDiskBlock() {
    QByteArray diskblock;
    diskblock.push_back('a');
    disk_block disk = *new disk_block(1, &diskblock);
    cout << disk.getBytes()->data() << endl;
    cout << disk.getBytes() << endl;
}


void testFileAllocationTable() {
    char chars[] = {-1, -1, -1, 4, 9, 0, 7, 8, -1, 12, 11, -1, 13, -1, 0, 0};
    vector<char>* bytes = new vector<char>;
    bytes->assign(chars, chars + 16);
    file_allocation_table fileAllocationTable(bytes);
    vector<item>* items = fileAllocationTable.getItemsStartWith(3);
    for (vector<item>::iterator it = items->begin(); it != items->end(); it++) {
        cout << it->next << endl;
    }

}

void testQByteArray(QByteArray *bytes0, int offset, int length) {
    QString qstr = *bytes0;
    string str = qstr.toStdString().substr(static_cast<unsigned long long>(offset), static_cast<unsigned long long>(length));
    QString qstr0 = QString::fromStdString(str);
    QByteArray bytes1 = qstr0.toLatin1();
    cout << bytes1[0] << endl;
}

/**
 * 初始化磁盘管理器
 */
void testDiskManager() {
    QFile file(disk_constant::DISK_NAME);
    file.open(QIODevice::ReadOnly);
    char buf[file_allocation_table_constant::LENGTH];
    file.read(buf, file_allocation_table_constant::LENGTH);
    file.close();
    vector<char>* bytes = new vector<char>;

    for (int i = 0; i < file_allocation_table_constant::LENGTH; i++) {
        bytes->push_back(buf[i]);
//        cout << "dasdas" << endl;
//        cout << buf[i] << endl;
    }
    file_allocation_table* fileAllocationTable = new file_allocation_table(bytes);
    disk_manager diskManager(fileAllocationTable);
}

int main()
{
    testDiskManager();

    return 0;
}
