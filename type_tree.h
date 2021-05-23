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

    TypeTree(): typeID(0), models_tree() {}

    friend bool operator==(const TypeTree& type_tree1, const TypeTree& type_tree2);

    friend bool operator<(const TypeTree& type_tree1, const TypeTree& type_tree2);

    ~TypeTree() = default;

    TypeTree(int typeID, AvlTree<Model>* models_tree) // if you want to make dummy, just put NULL into models_tree
    : typeID(typeID), models_tree()
    {
        this->models_tree = models_tree; 
    }

    int getTypeID()
    {
        return this->typeID;
    }

    AvlTree<Model>* getModelsTree()
    {
        return this->models_tree;
    }

    AvlTree<Model>* getLowestModel()
    {
        return this->models_tree->getMinNode();
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
