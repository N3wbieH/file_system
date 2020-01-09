#include "byte_utils.h"

#include "QString"
#include "string"
#include "iostream"
#include "QLatin1String"
using namespace std;

byte_utils::byte_utils(){}

QByteArray byte_utils::subQByteArray(QByteArray bytes0, int offset, int length) {
    QString qstr = bytes0;
    string str = qstr.toStdString().substr(static_cast<unsigned long long>(offset), static_cast<unsigned long long>(length));
    QString qstr0 = QString::fromStdString(str);
    return  qstr0.toLocal8Bit();
}

bool* byte_utils::byteToBooleans(char b) {
    bool *booleans = new bool[8];
    int b0 = b;
    for (int i = 1; i <= 8; i++) {
        booleans[8 - i] = b0 % 2;
        b0 /= 2;
    }
    return booleans;
}

QString byte_utils::bytesToString(QByteArray bytes, int offset, int length) {
    char *chars = new char[length];
    for (int i = 0, j = 0; i < length; i++) {
        chars[j++] = bytes[i + offset];
    }
    QString string = QString::fromUtf8(chars);
    return string;
}
