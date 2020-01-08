#include "directory_tree.h"
#include "iostream"
using namespace  std;

const QString directory_tree::SEPARATOR =  "/";

const QString directory_tree::ROOT_PATH = "/";

directory_tree::directory_tree(file *rootFile) {
    this->root = new node(nullptr, new vector<node>, rootFile);
}

node* directory_tree::addNode(node* directory, node* file) {
    // 目录不存在
    if (directory == nullptr) {
        throw nullptr;
    }

    // 目录节点已经到达上限
    if (directory->children->size() >= file_constant::MAX_FILE_NUMBER_OF_DIRECTORY) {
        return nullptr;
    }

    directory->children->push_back(*file);
    return directory;
}

node* directory_tree::getNode(QString path) {

    // 路径不是以"/"为开头的，非法路径
    if (!path.startsWith(SEPARATOR)) {
        return nullptr;
    }

    // 解析路径
    QStringList directories = QString::fromStdString(
                path.trimmed().toStdString().substr(1)).split(SEPARATOR);

    node* node0 = root;
    for (int i = 1; i < directories.size(); i++) {
        for (int j = 0; j < static_cast<int>(node0->children->size()); j++) {
            node* child = &(*node0->children)[static_cast<unsigned long long>(j)];
            // 如果此文件是目录文件且文件名和路径相符合
            if (file_supporter::getFileName(child->file0) == directories[i]) {
                if (i == directories.size() - 1) {
                    return child;
                }
                // 如果此文件是目录
                if (child->file0->getFileAttribute()->isDirectory()) {
                    node0 = child;
                    break;
                }
            }
            // 如果遍历完目录还是没找到，说明该目录不存在
            if (j == static_cast<int>(node0->children->size() - 1)) {

                return nullptr;
            }
        }
        // 否则如果已经是最后一层了，表示找到文件
        if (i == static_cast<int>(directories.size() - 1)) {
            return nullptr;
        }
    }

    // 如果以上情况都不符合，代表该路径是根目录
    return node0;
}


/**
 * 增加一个子节点
 *
 * @param directory Node 目录节点
 * @param file File 文件节点
 * @return 添加节点之后的目录
 */
node* directory_tree::addNode(node* directory, file* file) {
    node* child;
    // 如果是目录就初始化子节点列表
    if (file->getFileAttribute()->isDirectory()) {
        child = new node(directory, new vector<node>, file);
    } else {
        child = new node(directory, nullptr, file);
    }
    return addNode(directory, child);
}

/**
 * 增加一个子节点
 *
 * @param directoryPath String 目录路径
 * @param file Node 要添加的节点
 * @return 添加节点之后的目录
 */
node* directory_tree::addNode(QString directoryPath, node* file) {
    return addNode(getNode(directoryPath), file);
}

/**
 * 增加一个子节点
 *
 * @param directoryPath String 目录路径
 * @param file File 节点的文件
 * @return 添加节点之后的目录
 * @exception NotFoundException,IndexOutOfBoundsException .
 */
node* directory_tree::addNode(QString directoryPath, file* file) {
    return addNode(getNode(directoryPath), file);
}


/**
 * 删除一个子节点
 * @param path 节点的绝对路径
 * @return Node 被删除的节点
 */
node* directory_tree::deleteNode(QString path) {
    int index = path.trimmed().lastIndexOf(SEPARATOR);
    QString directoryPath;
    QString fileName =
            QString::fromStdString(path.toStdString().substr(static_cast<unsigned long long>(index + 1)));
    // 该节点在根目录下
    if (index == 0) {
        directoryPath = ROOT_PATH;
    } else {
        directoryPath =
                QString::fromStdString(path.toStdString().substr(0, static_cast<unsigned long long>(index)));
    }
    return deleteNode(directoryPath, fileName);
}

/**
 * 删除一个节点
 *
 * @param directoryPath 目录路径
 * @param fileName 文件名
 * @return Node 被删除的节点
 * @throws NotFoundException 未找到文件
 */
node* directory_tree::deleteNode(QString directoryPath, QString fileName) {
    node* directory = getNode(directoryPath);
    if (directory == nullptr) {
        return nullptr;
    }

    vector<node>* vector0 = directory->children;
    for (vector<node>::iterator child = vector0->begin(); child != vector0->end(); child++) {
        if (fileName == file_supporter::getFileName(child->getFile())) {
            directory->children->erase(child);
            return &*child;
        }
    }
    return nullptr;
}

/**
 * 获取根节点
 * @return 根节点
 */
node* directory_tree::getRoot() {
    return root;
}

