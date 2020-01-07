#ifndef BYTE_UTILS_H
#define BYTE_UTILS_H
#include "QByteArray"

class byte_utils
{
public:
    byte_utils();

    static QByteArray subQByteArray(QByteArray *bytes0, int offset, int length);

};

#endif // BYTE_UTILS_H
