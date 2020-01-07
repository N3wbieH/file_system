#include "byte_utils.h"

#include "QString"
#include "string"

using namespace std;

byte_utils::byte_utils(){}

QByteArray byte_utils::subQByteArray(QByteArray *bytes0, int offset, int length) {
    QString qstr = *bytes0;
    string str = qstr.toStdString().substr(static_cast<unsigned long long>(offset), static_cast<unsigned long long>(length));
    QString qstr0 = QString::fromStdString(str);
    return  qstr0.toLatin1();
}
