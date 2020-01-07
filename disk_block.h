#ifndef DISK_BLOCK_H
#define DISK_BLOCK_H
#include "vector"
using namespace std;

class disk_block
{
private:
    int index;
    vector<char> bytes;

public:
    disk_block(int index, vector<char> bytes);
    vector<char> getBytes();

    void setBytes(vector<char> bytes);

    int getIndex();

    void setIndex(int index);
};

#endif // DISK_BLOCK_H
