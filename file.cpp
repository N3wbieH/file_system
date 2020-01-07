#include "file.h"


file::file(QString name, QString type, file_attribute* fileAttribute, int firstDiskBlockIndex, int length) {
    this->name = name;
    this->type = type;
    this->fileAttribute = fileAttribute;
    this->firstDiskBlockIndex = firstDiskBlockIndex;
    this->length = length;
}

QString file::getName() {
    return name;
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