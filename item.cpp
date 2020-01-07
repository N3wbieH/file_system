#include "item.h"
#include "QString"

item::item(int index, int next) : index(index), next(next) {}
QString item::toString() {
    QString str = "Item{";
    str += "index=";
    str += static_cast<char>(index);
    str += ", next=";
    str += " + static_cast<char>(next)";
    str += '}';
    return str;
}

