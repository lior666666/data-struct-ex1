#ifndef DS_H_
#define DS_H_
#include "models_array.h"
#include "avl_tree.h"
#include "library.h"
#include "type_tree.h"
#include <iostream>

class CarDealershipManager
{
    AvlTree<ModelsArray> types_tree;
    AvlTree<Model> models_tree;
    AvlTree<TypeTree> zero_tree;
    int max_model_sales;
    Model* best_seller_model;
public:
    CarDealershipManager()
    {
        this->types_tree = *(new AvlTree<ModelsArray>());
        this->models_tree = *(new AvlTree<Model>());
        this->zero_tree = *(new AvlTree<TypeTree>());
        this->max_model_sales = 0;
        this->best_seller_model = NULL;
    };
    ~CarDealershipManager()
    {
        delete &(this->types_tree);
        delete &(this->models_tree);
        delete &(this->zero_tree);
    };
    StatusType AddCarType(int typeID, int numOfModels)
    {
        
    };
    StatusType RemoveCarType(int typeID)
    {
        ModelsArray* dummy_models_array = new ModelsArray(typeID); //o(1)
        ModelsArray* type_to_delete = this->types_tree.getNodeData(*dummy_models_array); //o(log(n))
        Model model_to_delete;
        if (type_to_delete == NULL)
        {
            delete dummy_models_array;
            return FAILURE;
        }

        //********delete from zero_tree********
        TypeTree* dummy_tree = new TypeTree(typeID); //o(1)
        TypeTree* type_to_delete_in_zero_tree = this->zero_tree.getNodeData(*dummy_tree); //o(log(n))
        if (type_to_delete_in_zero_tree != NULL)
        {
            (*type_to_delete_in_zero_tree).getModelsTree().clearTree(); //o(m)
            this->zero_tree.removeElement(*type_to_delete_in_zero_tree); //o(log(n))
        }
        delete dummy_tree;
        //****************

        //********delete from models_tree********
        for (int i = 0; i < (*type_to_delete).getNumOfModels(); i++) //o(m)
        {
            model_to_delete = (*type_to_delete).getModelsArray()[i];
            if (model_to_delete.getScore() != 0)
            {
                this->models_tree.removeElement(model_to_delete); //o(log(M))
            }
            delete &model_to_delete; //delete each model in the array
        }
        //****************

        this->types_tree.removeElement(*type_to_delete); //o(log(n)), delete the type from types_tree
        delete dummy_models_array; //delete dummy
        return SUCCESS;
    };
};
#endif