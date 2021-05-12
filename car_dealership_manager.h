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
    AvlTree<Model>* lowest_model;
    AvlTree<TypeTree>* lowest_zero_model;
public:
    CarDealershipManager()
    {
        this->types_tree = *(new AvlTree<ModelsArray>());
        this->models_tree = *(new AvlTree<Model>());
        this->zero_tree = *(new AvlTree<TypeTree>());
        this->max_model_sales = 0;
        this->best_seller_model = NULL;
        this->lowest_model = NULL;
        this->lowest_zero_model = NULL;
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
            if ((*this->lowest_zero_model->getData()) == *type_to_delete_in_zero_tree)
            {
                this->lowest_zero_model = this->lowest_zero_model->getParent();
            }
            (*type_to_delete_in_zero_tree).getModelsTree().clearTree(); //o(m)
            this->zero_tree.removeElement(*type_to_delete_in_zero_tree); //o(log(n))
        }
        delete dummy_tree;
        //****************

        //********delete from models_tree********
        for (int i = 0; i < (*type_to_delete).getNumOfModels(); i++) //o(m)
        {
            model_to_delete = (*type_to_delete).getModelsArray()[i];
            if (*(this->lowest_model->getData()) == model_to_delete)
            {
                this->lowest_model = this->lowest_model->getParent();
            }
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
                Model* update_model = (*(this->models_tree.getNodeData(model_to_complain))).addComplain(months); //o(log(M))
                if (*(this->lowest_model->getData()) > *update_model)
                {
                    this->lowest_model = this->models_tree.getNode(*update_model);
                }
            }
            else //after the complain - the score is 0 so the model goes to zero_tree
            {
                if (*(this->lowest_model->getData()) == model_to_complain)
                {
                    this->lowest_model = this->lowest_model->getParent();
                }
                this->models_tree.removeElement(model_to_complain); //o(log(M))
                TypeTree* dummy_tree = new TypeTree(typeID, true); //o(1)
                TypeTree* type_in_zero_tree = this->zero_tree.getNodeData(*dummy_tree); //o(log(n))
                ///////////if type_in_zero_tree==null need to insert
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

    int GetZeroModelsInTypeTree(AvlTree<Model>* min_model, int numOfModels, int save_counter, int* types, int* models)
    {
        Model* model;
        AvlTree<Model>* right_model;
        while (min_model != NULL) // save models in type tree
        {
            model = min_model->getData(); //o(1)
            types[save_counter] = model->getTypeID();
            models[save_counter] = model->getModelID();
            save_counter++;
            if (save_counter == numOfModels)
            {
                return save_counter;
            }
            right_model = min_model->getRight();
            while (right_model != NULL) // save right branch
            {
                 model = right_model->getData(); //o(1)
                 types[save_counter] = model->getTypeID();
                 models[save_counter] = model->getModelID();
                save_counter++;
                 if (save_counter == numOfModels)
                 {
                     return save_counter;
                }
                right_model = (*right_model).getRight(); //o(1)
             }
            min_model = (*min_model).getParent(); //o(1)
        }
        return save_counter;
    }

    int GetZeroModels(int numOfModels, int save_counter, int* types, int* models)
    {
        TypeTree* type_tree;
        Model* model;
        AvlTree<TypeTree>* min_tree = this->lowest_zero_model;
        AvlTree<TypeTree>* right_tree;
        AvlTree<Model>* min_model;
        while (min_tree != NULL)
        {
            type_tree = min_tree->getData(); //o(1)
            min_model = type_tree->getLowestModel(); //o(1)
            save_counter = GetZeroModelsInTypeTree(min_model, numOfModels, save_counter, types, models);
            if (save_counter == numOfModels)
            {
                return save_counter;
            }
            right_tree = min_tree->getRight();
            while (right_tree != NULL) // print right branch
            {
                type_tree = right_tree->getData(); //o(1)
                min_model = type_tree->getLowestModel(); //o(1)
                save_counter = GetZeroModelsInTypeTree(min_model, numOfModels, save_counter, types, models);
                if (save_counter == numOfModels)
                {
                    return save_counter;
                }
                right_tree = (*right_tree).getRight(); //o(1)
            }
            min_tree = (*min_tree).getParent(); //o(1)
        }
        return save_counter;
    }

    StatusType GetWorstModels(int numOfModels, int* types, int* models)
    {
        Model* model;
        int save_counter = 0;
        bool is_print_zero = false;
        AvlTree<Model>* min_model = this->lowest_model;
        AvlTree<Model>* right_model;
        while (min_model != NULL)
        {
            model = min_model->getData(); //o(1)
            if (!is_print_zero && model->getScore() > 0) // first time of positive - check zero_tree
            {
                save_counter = GetZeroModels(numOfModels, save_counter, types, models);
                if (save_counter == numOfModels)
                {
                    return SUCCESS;
                }
                is_print_zero = true;
            }
            types[save_counter] = model->getTypeID();
            models[save_counter] = model->getModelID();
            save_counter++;
            if (save_counter == numOfModels)
            {
                return SUCCESS;
            }
            right_model = min_model->getRight();
            while (right_model != NULL) // print right branch
            {
                model = right_model->getData(); //o(1)
                if (!is_print_zero && model->getScore() > 0) // first time of positive - check zero_tree
                {
                    save_counter = GetZeroModels(numOfModels, save_counter, types, models);
                    if (save_counter == numOfModels)
                    {
                        return SUCCESS;
                    }
                    is_print_zero = true;
                }
                types[save_counter] = model->getTypeID();
                models[save_counter] = model->getModelID();
                save_counter++;
                if (save_counter == numOfModels)
                {
                    return SUCCESS;
                }
                right_model = (*right_model).getRight(); //o(1)
            }
            min_model = (*min_model).getParent(); //o(1)
        }
        return (save_counter == numOfModels) ? SUCCESS : FAILURE;
    }
};
#endif