#include "file_attribute.h"


file_attribute::file_attribute(bool readOnly, bool system, bool readWrite, bool directory) {
    this->readOnly = readOnly;
    this->system = system;
    this->readWrite = readWrite;
    this->directory = directory;
}

bool file_attribute::isReadOnly() {
    return readOnly;
}

bool file_attribute::isSystem() {
    return system;
}

bool file_attribute::isReadWrite() {
    return readWrite;
}

bool file_attribute::isDirectory() {
    return directory;
}
