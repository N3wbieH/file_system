#include "file.h"
#include "QString"
#include "iostream"
using namespace std;

file::file(const file &file0) {
    this->name = file0.name;
    this->type = file0.type;
    this->fileAttribute = new file_attribute(file0.fileAttribute->isReadOnly(), file0.fileAttribute->isSystem(),
                                             file0.fileAttribute->isReadWrite(), file0.fileAttribute->isDirectory());
    this->firstDiskBlockIndex = file0.firstDiskBlockIndex;
    this->length = file0.length;
}

file::file(QString name, QString type, file_attribute* fileAttribute, int firstDiskBlockIndex, int length) {
    this->name = name;
    this->type = type;
    this->fileAttribute = fileAttribute;
    this->firstDiskBlockIndex = firstDiskBlockIndex;
    this->length = length;
}

QString file::getName() {
    QString qs = name;
    return qs;
}

QString file::getType() {
    return type;
}

file_attribute* file::getFileAttribute() {
    return fileAttribute;
}

int file::getFirstDiskBlockIndex() {
    return firstDiskBlockIndex;
}

int file::getLength() {
    return length;
}
