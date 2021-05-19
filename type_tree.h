#ifndef TYPE_TREE_H_
#define TYPE_TREE_H_
#include "avl_tree.h"
#include "model.h"
#include <iostream>

class TypeTree
{
    int typeID;
    int number_of_models;
    AvlTree<Model>* models_tree;
public:
    TypeTree(): typeID(0), number_of_models(0), models_tree(NULL) {}

    friend bool operator==(const TypeTree& type_tree1, const TypeTree& type_tree2);

    friend bool operator<(const TypeTree& type_tree1, const TypeTree& type_tree2);

    friend std::ostream& operator<<(std::ostream& os, const TypeTree& c);

    ~TypeTree() = default;

    TypeTree(int typeID, int number_of_models, AvlTree<Model>* models_tree) // if you want to make dummy, just put NULL into models_tree
    : typeID(typeID), number_of_models(number_of_models)
    {
        this->models_tree = models_tree; 
    }

    int getTypeID()
    {
        return this->typeID;
    }

    bool insertModel(Model model)
    {
        bool is_inserted = this->models_tree->insertElement(model);
        if (is_inserted)
        {
            this->number_of_models++;
        }
        return is_inserted;
    }

    bool removeModel(Model model)
    {
        bool is_remove = this->models_tree->removeElement(model);
        if (is_remove)
        {
            this->number_of_models--;
        }
        return is_remove;
    }

    AvlTree<Model>* getModelsTree()
    {
        return this->models_tree;
    }

    AvlTree<Model>* getLowestModel()
    {
        return this->models_tree->getMinNode();
    }

    int getModelsTreeLength()
    {
        return this->number_of_models;
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

std::ostream& operator<<(std::ostream& os, const TypeTree& c)
{
    return os << "typeTree:" << c.typeID << ":" << c.number_of_models << "***";
}
#endif