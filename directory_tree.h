#ifndef DIRECTORY_TREE_H
#define DIRECTORY_TREE_H

#include "node.h"
#include "QString"
#include "qstringlist.h"
#include "file_constant.h"
#include "file_supporter.h"
using namespace std;

/**
 * 描述: 目录树
 */
class directory_tree
{
private:
    /**
     * 目录分隔符
     */
    static const QString SEPARATOR;

    /**
     * 根目录路径
     */
    static const QString ROOT_PATH;

    /**
     * 根目录
     */
    node* root;

public:
    directory_tree();

    directory_tree(file *rootFile);

    /**
     * 增加一个子节点
     *
     * @param directory Node 目录节点
     * @param file Node 子节点
     * @return 添加节点之后的目录
     * @throws NotFoundException 目录不存在
     * @throws IndexOutOfBoundsException 目录节点数已到达上限
     */
    node* addNode(node* directory, node* file);

    /**
     * 增加一个子节点
     *
     * @param directory Node 目录节点
     * @param file File 文件节点
     * @return 添加节点之后的目录
     */
    node* addNode(node* directory, file* file);

    /**
     * 增加一个子节点
     *
     * @param directoryPath String 目录路径
     * @param file Node 要添加的节点
     * @return 添加节点之后的目录
     */
    node* addNode(QString directoryPath, node* file);

    /**
     * 增加一个子节点
     *
     * @param directoryPath String 目录路径
     * @param file File 节点的文件
     * @return 添加节点之后的目录
     * @exception NotFoundException,IndexOutOfBoundsException .
     */
    node* addNode(QString directoryPath, file* file);

    /**
     * 寻找一个文件，可以是目录或者是普通文件
     *
     * @param path 文件路径
     * @return Node 文件节点
     */
    node* getNode(QString path);

    /**
     * 删除一个子节点
     * @param path 节点的绝对路径
     * @return Node 被删除的节点
     */
    node* deleteNode(QString path);

    /**
     * 删除一个节点
     *
     * @param directoryPath 目录路径
     * @param fileName 文件名
     * @return Node 被删除的节点
     * @throws NotFoundException 未找到文件
     */
    node* deleteNode(QString directoryPath, QString fileName);

    /**
     * 获取根节点
     * @return 根节点
     */
    node* getRoot();



};

#endif // DIRECTORY_TREE_H
