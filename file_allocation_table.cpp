#include "file_allocation_table.h"
#include "item.h"
#include "iostream"
using namespace std;

file_allocation_table::file_allocation_table(vector<char> items) {
    init(items);
}

void file_allocation_table::init(vector<char> items) {
    for (int i = 0; i < static_cast<int>(items.size()); i++) {
        item item(i, items[static_cast<unsigned long long >(i)]);
        cout << item.index << " " << item.next << endl;
        this->items.push_back(item);
    }
}

item* file_allocation_table::getEmptyItem() {
    for (int i = file_allocation_table_constant::NUMBER_OF_FAT_DISK_BLOCKS;
         i < file_allocation_table_constant::LENGTH; i++) {
        if (getItem(i)->next == file_allocation_table_constant::EMPTY) {
            return getItem(i);
        }
    }
    return nullptr;
}

void file_allocation_table::updateItem(item *item) {
    items.insert(items.begin() + item->index, *item);
}

void file_allocation_table::releaseItem(int previous) {
    item *previousItem = getItem(previous);
    item *releasedItem = getItem(previousItem->next);
    item *newPreviousItem = new item(previous, releasedItem->next);
    item *newReleasedItem = new item(releasedItem->index, file_allocation_table_constant::EMPTY);
    updateItem(newPreviousItem);
    updateItem(newReleasedItem);
}

item* file_allocation_table::getItem(int index) {
    return &items[static_cast<unsigned long long>(index)];
}

item* file_allocation_table::allocateItem() {
   item *item0 = getEmptyItem();
   // 没有空闲磁盘块
   if (item0 == nullptr) {
       return nullptr;
   }

   // 更新文件分配表
   item *newItem = new item(item0->index, file_allocation_table_constant::END);
   updateItem(newItem);
   return newItem;
}


item* file_allocation_table::allocateItem(int previous) {
    item *item0 = getEmptyItem();
    // 没有空闲磁盘块
    if (item0 == nullptr) {
        return nullptr;
    }

    // 前一个文件分配表项
    item* newPreviousItem = new item(previous, item0->index);
    // 新分配的文件分配表项
    item* newItem = new item(item0->index, getItem(previous)->next);
    // 更新文件分配表
    updateItem(newPreviousItem);
    updateItem(newItem);
    return newItem;
}

void file_allocation_table::releaseItemsStartWith(int start) {
    releaseItemsPreviousWith(start);
    updateItem(new item(start, 0));
}

void file_allocation_table::releaseItemsPreviousWith(int previous) {
    while (getItem(previous)->next != -1) {
        releaseItem(previous);
    }
}

QString file_allocation_table::toString() {
    QString str = "FileAllocationTable{";
    str += "items=";
    for (vector<item>::iterator it = items.begin(); it != items.end(); ++it) {
        str += it->toString();
    }
    str += '}';
    return str;
}

vector<item> file_allocation_table::getItemsStartWith(int startIndex) {
    // 该下标指向文件分配表项
    if (startIndex < file_allocation_table_constant::NUMBER_OF_FAT_DISK_BLOCKS) {
        vector<item> vector;
        vector.push_back(*getItem(startIndex));
        return vector;
    }

    // 该磁盘分配表项指向空盘块
    if (getItem(startIndex)->next == file_allocation_table_constant::EMPTY) {
        vector<item> vector;
        vector.push_back(*getItem(startIndex));
        return vector;
    }

    // 生成该文件的盘块链
    vector<item> itemList;
    item *item0 = getItem(startIndex);
    itemList.push_back(*item0);
    while (item0->next != file_allocation_table_constant::END) {
        cout << item0->next << endl;
        item0 = getItem(item0->next);
        itemList.push_back(*item0);
    }
    return itemList;
}

