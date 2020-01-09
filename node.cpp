#include "node.h"
#include "iostream"
using namespace std;

node::node(node *parent, vector<node>* children, file* file) {
    this->file0 = file;
    this->parent = parent;
    this->children = children;
}

node* node::getParent() {
    return parent;
}

void node::setParent(node* parent) {
    this->parent = parent;
}

vector<node>* node::getChildren() {
    return children;
}

void node::setChildren(vector<node>* children) {
    this->children = children;
}

file* node::getFile() {
    return file0;
}

void node::setFile(file* file) {
    file0 = file;
}
