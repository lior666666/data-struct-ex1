#ifndef DS_H_
#define DS_H_
#include "models_array.h"
#include "avl_tree.h"
#include <iostream>

class CarDealershipManager
{
    AvlTree<ModelsArray>* types_tree;
    AvlTree<Model>* models_tree;
    AvlTree<AvlTree<Model> >* zero_tree; // the space is important!!!
    int max_model_sales;
    Model* best_seller_model;
public:
    CarDealershipManager()
    {
        this->types_tree = new AvlTree<ModelsArray>();
        this->models_tree = new AvlTree<Model>();
        this->zero_tree = new AvlTree<AvlTree<Model> >();
        this->max_model_sales = 0;
        this->best_seller_model = NULL;
    };
    ~CarDealershipManager()
    {
        delete this->types_tree;
        delete this->models_tree;
        delete this->zero_tree;
    };

    //all the function...
    
};
#endif