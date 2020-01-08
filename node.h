#ifndef NODE_H
#define NODE_H
#include "vector"
#include "file.h"
using namespace std;

class node
{

public:
    node *parent;

    vector<node>* children;

    file* file0;

    node(node *parent, vector<node>* children, file* file);

    node* getParent();

    void setParent(node* parent);

    vector<node>* getChildren();

    void setChildren(vector<node>* children);

    file* getFile();

    void setFile(file* file);
};

#endif // NODE_H
