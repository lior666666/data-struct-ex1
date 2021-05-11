#ifndef TYPE_TREE_H_
#define TYPE_TREE_H_
#include "avl_tree.h"
#include "model.h"
#include <iostream>

class TypeTree
{
    int typeID;
    AvlTree<Model>* models_tree;
public:
    TypeTree() = default;
    ~TypeTree()
    {
        (*this->models_tree).clearTree();
    };
    TypeTree(const TypeTree& type_tree) = default;
    friend bool operator==(const TypeTree& type_tree1, const TypeTree& type_tree2);
    friend bool operator<(const TypeTree& type_tree1, const TypeTree& type_tree2);
    TypeTree(int typeID)
    {
        this->typeID = typeID;
        this->models_tree = NULL;
    }
    AvlTree<Model> getModelsTree()
    {
        return *(this->models_tree);
    }
};

bool operator==(const TypeTree& type_tree1, const TypeTree& type_tree2)
{
    return type_tree1.typeID == type_tree2.typeID;
}

bool operator<(const TypeTree& type_tree1, const TypeTree& type_tree2)
{
    return type_tree1.typeID < type_tree2.typeID;
}

bool operator>(const TypeTree& type_tree1, const TypeTree& type_tree2)
{
    return !(type_tree1 < type_tree2) && !(type_tree1 == type_tree2);
}
#endif