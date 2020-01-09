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
#include "qdatastream.h"
#include "file_manager.h"
#include "file.h"
#include "byte_utils.h"
#include "QDebug"
using namespace std;

//void testDiskBlock() {
//    QByteArray diskblock;
//    diskblock.push_back('a');
//    disk_block disk = *new disk_block(1, diskblock);
//    cout << disk.getBytes().data() << endl;
////    cout << disk.getBytes() << endl;
//}



//void testQByteArray(QByteArray *bytes0, int offset, int length) {
//    QString qstr = *bytes0;
//    string str = qstr.toStdString().substr(static_cast<unsigned long long>(offset), static_cast<unsigned long long>(length));
//    QString qstr0 = QString::fromStdString(str);
//    QByteArray bytes1 = qstr0.toLatin1();
//    cout << bytes1[0] << endl;
//}

/**
 * 初始化磁盘管理器
 */
void testDiskManager() {
    file_manager fileManager;
    file* file0 = fileManager.getFile("/usr");
    vector<file>* fileVector = fileManager.getFileList("/");
    for (vector<file>::iterator file0 = fileVector->begin(); file0 != fileVector->end(); file0++) {
            qDebug() << file0->getName();
//        if (file0->getFileAttribute()->isDirectory()) {
//            qDebug() << "---------------";
//            qDebug() << fileManager.getFile("/" + file0->getName());
//            qDebug() << fileManager.getFileList("/" + file0->getName())->size();
//            qDebug() << file0->getName();
//            vector<file>* fileVector0 = fileManager.getFileList("/" + file0->getName());
//            for (vector<file>::iterator file1 = fileVector0->begin(); file1 != fileVector0->end(); file1++) {

//                qDebug() << file1->getName() + file1->getType();
//            }
//        }
    }
    fileManager.createFile("/", "abc", true);


//    if ((file0->getFileAttribute()->isDirectory())) {
//        cout << "dsadas" << endl;
//    }
//    QString content = fileManager.readFile("/usr/dir/tx.t");
//    qDebug() << content << endl;

//    cout << fileManager.getDiskUsage() << endl;



//    vector<file>* fileList = fileManager.getFile("/");
//    cout << fileList->size() << endl;
//     cout << fileManager->directoryTree->getRoot()->getFile()->getType().toStdString() << endl;
//       printf("%d\n", fileManager.directoryTree->getRoot()->getChildren()->size());
//       printf("\n");
//    cout << "2222"<< endl;
//    vector<file>* fileList = fileManager->getFileList("/usr/dir/");
//    cout << "31231"<< endl;
//    cout << (fileList)->size() << endl;
//    cout << "31231"<< endl;
//    for (file file1 : *fileList) {
//        QString qs = file1.getName();
//        cout << qs.data() << endl;
//    }
//   cout << "1111"<< endl;
}

//int main()
//{
//    testDiskManager();

//    return 0;
//}

