#include "disk_block.h"

disk_block::disk_block(int index, vector<char> bytes) {
    this->index = index;
    this->bytes = bytes;
}
vector<char> disk_block::getBytes() {
    return bytes;
}

void disk_block::setBytes(vector<char> bytes) {
    this->bytes = bytes;
}

int disk_block::getIndex() {
    return index;
}

void disk_block::setIndex(int index) {
    this->index = index;
}
