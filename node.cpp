#include "node.h"


node::node(node* parent, vector<node> *children, file *file) {
    this->parent = parent;
    this->children = children;
    this->file0 = file;
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

void node::setFile(file *file) {
    this->file0 = file;
}
