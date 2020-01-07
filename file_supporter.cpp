#include "file_supporter.h"

file_supporter::file_supporter()
{

}

int file_supporter::findEmptySpaceOfDiskBlock(vector<char> *bytes, int pace, char symbol) {
    for (int i = 0; i < static_cast<int>(bytes->size()); i += pace) {
        if (bytes->data()[i] == symbol) {
            return i;
        }
    }
    return -1;
}
QString file_supporter::getFileName(file* file) {
    if (file->getFileAttribute()->isDirectory()) {
        return file->getName().trimmed();
    }
    if (string_utils::isBlank(file->getType())) {
        return file->getName().trimmed();
    }
    return file->getName().trimmed() + file_constant::FILE_NAME_SEPARATOR + file->getType().trimmed();
}
QString file_supporter::getFileName(QString path) {
    int index = path.lastIndexOf("/");
    if (index == -1) {
        return path;
    }
    string str = path.toStdString().substr(static_cast<unsigned long long>(index + 1));
    return QString::fromStdString(str);
}
