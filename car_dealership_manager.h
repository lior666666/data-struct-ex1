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
        TypeTree* dummy_tree = new TypeTree(typeID, true); //o(1)
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

    StatusType MakeComplaint(int typeID, int modelID, int months)
    {
        ModelsArray* dummy_models_array = new ModelsArray(typeID); //o(1)
        ModelsArray* type_to_complain = this->types_tree.getNodeData(*dummy_models_array); //o(log(n))
        if (type_to_complain == NULL)
        {
            delete dummy_models_array;
            return FAILURE;
        }
        int models_array_length = type_to_complain->getNumOfModels();
        if (models_array_length <= modelID)
        {
            delete dummy_models_array;
            return INVALID_INPUT;
        }
        Model* models_array = type_to_complain->getModelsArray();
        Model model_to_complain = models_array[modelID];
        int prev_score = model_to_complain.getScore();
        model_to_complain.addComplain(months); //o(1)
        int new_score = model_to_complain.getScore();
        if (prev_score != 0) //the model in models_tree
        {
            if (new_score != 0) //after the complain - the score is not 0 so the model stay in models_tree
            {
                (*(this->models_tree.getNodeData(model_to_complain))).addComplain(months); //o(log(M))
            }
            else //after the complain - the score is 0 so the model goes to zero_tree
            {
                this->models_tree.removeElement(model_to_complain); //o(log(M))
                TypeTree* dummy_tree = new TypeTree(typeID, true); //o(1)
                TypeTree* type_in_zero_tree = this->zero_tree.getNodeData(*dummy_tree); //o(log(n))
                Model* model_to_complain_in_zero_tree = new Model(model_to_complain); //o(1)
                model_to_complain_in_zero_tree->addComplain(months); //o(1)
                if (!type_in_zero_tree->insertModel(*model_to_complain_in_zero_tree)) //o(log(M))
                {
                    return FAILURE; //for debugging. It should not happen!!
                }
                delete dummy_tree;
            }
        }
        else //the model in zero_tree
        {
            TypeTree* dummy_tree = new TypeTree(typeID, true); //o(1)
            TypeTree* type_in_zero_tree = this->zero_tree.getNodeData(*dummy_tree); //o(log(n))
            if (!type_in_zero_tree->removeModel(model_to_complain)); //o(log(M))
            {
                return FAILURE; //for debugging. It should not happen!!
            }
            Model* model_to_complain_in_models_tree = new Model(model_to_complain); //o(1)
            model_to_complain_in_models_tree->addComplain(months); //o(1)
            this->models_tree.insertElement(*model_to_complain_in_models_tree); //o(log(M))
            delete dummy_tree;
        }
        delete dummy_models_array; //delete dummy
        return SUCCESS;
    };

    int GetNegativeScoreModels(AvlTree<Model>* models_tree, int models_to_save, int* save_counter, int* types, int* models)
    {
        if (models_to_save == 0)
        {
            return models_to_save; //the number of models that left to save
        }
        if(models_tree->getLeft() != NULL)
        {
            models_to_save = GetNegativeScoreModels(models_tree->getLeft(), models_to_save, save_counter, types, models);
        }
        if (models_to_save == 0)
        {
            return models_to_save; //the number of models that left to save
        }
        Model* model = models_tree->getData();
        if (model->getScore() >= 0)
        {
            return models_to_save;
        }
        types[*save_counter] = model->getTypeID();
        models[*save_counter] = model->getModelID();
        (*save_counter)++;
        models_to_save--;
        if(models_tree->getRight() != NULL)
        {
            models_to_save = GetNegativeScoreModels(models_tree->getRight(), models_to_save, save_counter, types, models);
        }
        return models_to_save;
    }

    StatusType GetWorstModels(int numOfModels, int* types, int* models)
    {
        int save_counter = 0;;
        if(this->models_tree.getLeft() != NULL)
        {
            GetNegativeScoreModels(this->models_tree.getLeft(), numOfModels, &save_counter, types, models);
        }
    }
};
#endif