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
    TypeTree(const TypeTree& type_tree) = default;
    friend bool operator==(const TypeTree& type_tree1, const TypeTree& type_tree2);
    friend bool operator<(const TypeTree& type_tree1, const TypeTree& type_tree2);
    ~TypeTree()
    {
        (*this->models_tree).clearTree();
    };
    TypeTree(int typeID, bool is_dummy)
    {
        this->typeID = typeID;
        if (is_dummy)
        {
            this->models_tree = NULL;
        }
        else
        {
            this->models_tree = new AvlTree<Model>();
        }
    }
    bool insertModel(Model model)
    {
        return this->models_tree->insertElement(model);
    }
    bool removeModel(Model model)
    {
        return this->models_tree->removeElement(model);
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