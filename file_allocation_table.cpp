#include "file_allocation_table.h"
#include "item.h"
#include "QFile"
#include "iostream"
#include "disk_constant.h"

using namespace std;

file_allocation_table::file_allocation_table() {
    init();
}

void file_allocation_table::init() {
    QFile file0(disk_constant::DISK_NAME);
    file0.open(QIODevice::ReadOnly);
    char buf[file_allocation_table_constant::LENGTH];
    file0.read(buf, file_allocation_table_constant::LENGTH);
    file0.close();
    vector<item> items0;
    for (int i = 0; i < file_allocation_table_constant::LENGTH; i++) {
        items0.push_back(*new item(i, static_cast<int>(buf[i])));
    }
    items = &items0;
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
    (*items)[static_cast<unsigned long long>(item->index)] = *item;
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
    return &(*items)[static_cast<unsigned long long>(index)];
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


vector<item>* file_allocation_table::getItemsStartWith(int startIndex) {
    // 该下标指向文件分配表项
    if (startIndex < file_allocation_table_constant::NUMBER_OF_FAT_DISK_BLOCKS) {
        vector<item>* vector0 = new vector<item>;
        vector0->push_back(*getItem(startIndex));
        return vector0;
    }

    // 该磁盘分配表项指向空盘块
    if (getItem(startIndex)->next == file_allocation_table_constant::EMPTY) {
        vector<item>* vector0 = new vector<item>;
        vector0->push_back(*getItem(startIndex));
        return vector0;
    }

    // 生成该文件的盘块链
    vector<item>* itemList = new vector<item>;
    item *item0 = getItem(startIndex);
    itemList->push_back(*item0);
    while (item0->next != file_allocation_table_constant::END) {
        item0 = getItem(item0->next);
        itemList->push_back(*item0);
    }
    return itemList;
}

