#include "disk_block.h"

disk_block::disk_block(int index, QByteArray *bytes) {
    this->index = index;
    this->bytes = bytes;
}
QByteArray *disk_block::getBytes() {
    return bytes;
}

void disk_block::setBytes(QByteArray *bytes) {
    this->bytes = bytes;
}

int disk_block::getIndex() {
    return index;
}

void disk_block::setIndex(int index) {
    this->index = index;
}
