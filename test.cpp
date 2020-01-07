#include "test.h"
#include "disk_block.h"
#include "item.h"
#include "iostream"
#include "file_allocation_table.h"
#include "vector"
using namespace std;

void testDiskBlock() {
    vector<char> diskblock;
    diskblock.push_back(49);
    disk_block disk = *new disk_block(1, diskblock);
    cout << static_cast<int>(disk.getBytes()[0]) << endl;
    cout << static_cast<char>(disk.getBytes()[0]) << endl;
}


void testFileAllocationTable() {
        char chars[] = {-1, -1, -1, 4, 9, 0, 7, 8, -1, 12, 11, -1, 13, -1, 0, 0};
        vector<char> bytes;
        bytes.assign(chars, chars + 16);
        file_allocation_table fileAllocationTable(bytes);
        vector<item> items = fileAllocationTable.getItemsStartWith(3);
        for (vector<item>::iterator it = items.begin(); it != items.end(); it++) {
            cout << it->next << endl;
        }

    }
int main()
{
testFileAllocationTable();

    return 0;
}
