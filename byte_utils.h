#ifndef BYTE_UTILS_H
#define BYTE_UTILS_H
#include "QByteArray"
#include "QString"

class byte_utils
{
public:
    byte_utils();

    static QByteArray subQByteArray(QByteArray *bytes0, int offset, int length);

    /**
     * 字节类型转换成布尔型数组
     *
     * @param b 字节
     * @return boolean[]
     */
    static bool* byteToBooleans(char b);

    /**
     * 字节数组转字符串
     *
     * @param bytes 字节数组
     * @param offset 起始下标
     * @param length 长度
     * @return String 字符串
     */
    static QString bytesToString(QByteArray* bytes, int offset, int length);

};

#endif // BYTE_UTILS_H
