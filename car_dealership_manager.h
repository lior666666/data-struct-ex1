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
    Model best_seller_model;
public:
    CarDealershipManager() : max_model_sales(0) {};

    ~CarDealershipManager() = default;

    StatusType AddCarType(int typeID, int numOfModels)
    {
        // !!!remember to check parameter in library.cpp!!!!
        ModelsArray new_type = ModelsArray(typeID, numOfModels); // maybee a "new" is needed, but I don't think so. // O(m)
        if (types_tree.insertElement(new_type) == false)
        {
            return FAILURE; //already exist typeID
        }
        AvlTree<Model>* models_tree = new AvlTree<Model>(new_type.getModelsArray(), new_type.getNumOfModels()); // O(m)  special algoritm for building AVL tree. 

        TypeTree new_tree_node = TypeTree(typeID, numOfModels, models_tree);    
        zero_tree.insertElement(new_tree_node); // o(log n)
        return SUCCESS;
    };

    StatusType RemoveCarType(int typeID)
    {
        ModelsArray dummy_models_array = ModelsArray(typeID); //o(1)
        ModelsArray* type_to_delete = this->types_tree.getNodeData(dummy_models_array); //o(log(n))
        Model model_to_delete;
        if (type_to_delete == NULL)
        {
            return FAILURE;
        }

        //********delete from zero_tree********
        TypeTree dummy_tree = TypeTree(typeID, 0, NULL); //o(1)
        TypeTree* type_to_delete_in_zero_tree = this->zero_tree.getNodeData(dummy_tree); //o(log(n))
        if (type_to_delete_in_zero_tree != NULL)
        {
            type_to_delete_in_zero_tree->getModelsTree().clearTree(); //o(m)
            this->zero_tree.removeElement(*type_to_delete_in_zero_tree); //o(log(n))
        }
        //****************

        //********delete from models_tree********
        for (int i = 0; i < (*type_to_delete).getNumOfModels(); i++) //o(m)
        {
            model_to_delete = (*type_to_delete).getModelsArray()[i];
            if (model_to_delete.getScore() != 0)
            {
                this->models_tree.removeElement(model_to_delete); //o(log(M))
            }
        }
        //****************

        this->types_tree.removeElement(*type_to_delete); //o(log(n)), delete the type from types_tree
        return SUCCESS;
    };

    StatusType SellCar(int typeID, int modelID)
    {
        ModelsArray dummy_id = ModelsArray(typeID);
        AvlTree<ModelsArray>* real_Node_id = types_tree.getNode(dummy_id); // log n
        if (real_Node_id == NULL)
        {
            return FAILURE;
        }
        int models_array_length = real_Node_id->getData().getNumOfModels();
        if (models_array_length <= modelID)
        {
            return INVALID_INPUT;
        }
        ModelsArray* model_to_change = real_Node_id->getNodeData(dummy_id); // log n
        model_to_change->getModelsArray()[modelID].addSale(); // adding 1 sale + chanching total scores. 

        Model updated_model =  model_to_change->getModelsArray()[modelID]; // the new model after update of sales. 
        model_to_change->updateBestSellerModel(updated_model); // checks inside the function if it should update the min. 
        if(updated_model.getNumOfSales() > best_seller_model.getNumOfSales())
        {
            this->best_seller_model = updated_model; 
        }
        else if (updated_model.getNumOfSales() == best_seller_model.getNumOfSales()) 
        {
            if (updated_model.getTypeID() < best_seller_model.getTypeID())
            {
                this->best_seller_model = updated_model;
            }
            else if (updated_model.getTypeID() == best_seller_model.getTypeID()) 
            {
                if (updated_model.getModelID() < best_seller_model.getModelID())
                {
                    this->best_seller_model = updated_model;
                }
            }
        }

        TypeTree dummy_type_tree_id = TypeTree(typeID, 0, NULL); 
        AvlTree<TypeTree>* real_Node_zero_tree_id = zero_tree.getNode(dummy_type_tree_id); // log n
        Model dummy_model = Model(typeID, modelID);
        bool zero_is_empty = true;
        // means there are models with 0 scores under this ID.
        if(real_Node_zero_tree_id != NULL)  
        { 
            AvlTree<Model> zero_models_tree = real_Node_zero_tree_id->getData().getModelsTree();  
            AvlTree<Model>* real_Model_Node = zero_models_tree.getNode(dummy_model); // log m
            if(real_Model_Node != NULL) // if the update model in the type tree
            {
                real_Node_zero_tree_id->getData().removeModel(dummy_model); // log m

                if(zero_models_tree.isEmpty()) // in case there are no 0 score models under this ID. 
                {
                    zero_tree.removeElement(dummy_type_tree_id);
                }
                models_tree.insertElement(updated_model); // log M 
                zero_is_empty = false; 
            }
        }
        // in case zero trees don't have this model.
        if(zero_is_empty == true || real_Node_zero_tree_id == NULL) 
        {
            if(models_tree.removeElement(dummy_model) == false)//  log M 
            {
                return FAILURE;
            }
            if(updated_model.getScore() == 0) // will add him to zero tree. 
            {
                if(real_Node_zero_tree_id == NULL) // IN CASE THERE Is NO NODE WITH 0 MODELS UNDER THIS ID. 
                {
                    AvlTree<Model>* zero_models_tree =  new AvlTree<Model>; 
                    zero_models_tree->insertElement(updated_model);
                    AvlTree<Model>* low_zero_model = zero_models_tree->getMinNode(); // O(1) in this specific case. 
                    TypeTree new_type_tree_node = TypeTree(typeID, 1, zero_models_tree); 
                    this->zero_tree.insertElement(new_type_tree_node); 
                }
                else // there are models under this ID with zero.
                {
                    real_Node_zero_tree_id->getData().insertModel(updated_model); // log m
                }
            }
            else // we need to insert it back to models tree
            {
                models_tree.insertElement(updated_model);
            }
        }
        return SUCCESS; 
    }

    StatusType MakeComplaint(int typeID, int modelID, int months)
    {
        ModelsArray dummy_models_array = ModelsArray(typeID); //o(1)
        ModelsArray* type_to_complain = this->types_tree.getNodeData(dummy_models_array); //o(log(n))
        if (type_to_complain == NULL)
        {
            return FAILURE;
        }
        int models_array_length = type_to_complain->getNumOfModels();
        if (models_array_length <= modelID)
        {
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
                if(models_tree.removeElement(model_to_complain) == false)//  log M 
                {
                    return FAILURE;
                }
                Model* update_model = this->models_tree.getNodeData(model_to_complain)->addComplain(months); //o(log(M))
                models_tree.insertElement(*update_model);
            }
            else //after the complain - the score is 0 so the model goes to zero_tree
            {
                //remove from models_tree
                this->models_tree.removeElement(model_to_complain); //o(log(M))

                //add to zero_tree
                TypeTree dummy_tree = TypeTree(typeID, 0, NULL); //o(1)
                TypeTree* type_in_zero_tree = this->zero_tree.getNodeData(dummy_tree); //o(log(n))
                Model model_to_complain_in_zero_tree = Model(model_to_complain); //o(1)
                model_to_complain_in_zero_tree.addComplain(months); //o(1)
                if (type_in_zero_tree == NULL) // type id is not in the zero_tree --> this is the first zero score for that type
                {
                    AvlTree<Model>* zero_models_tree =  new AvlTree<Model>; 
                    zero_models_tree->insertElement(model_to_complain_in_zero_tree);
                    TypeTree type_in_zero_tree = TypeTree(typeID, 1, zero_models_tree); 
                    this->zero_tree.insertElement(type_in_zero_tree); 
                }
                else
                {
                    type_in_zero_tree->insertModel(model_to_complain_in_zero_tree); //o(log(n))
                }
            }
        }
        else //the model in zero_tree
        {
            TypeTree dummy_tree = TypeTree(typeID, 0, NULL); //o(1)
            AvlTree<TypeTree>* type_in_zero_tree = this->zero_tree.getNode(dummy_tree); // log n
            if (!type_in_zero_tree->getData().removeModel(model_to_complain)); //o(log(M))
            {
                return FAILURE; //for debugging. It should not happen!!
            }
            if (type_in_zero_tree->isEmpty())
            {
                this->zero_tree.removeElement(type_in_zero_tree->getData()); //o(log(n)
            }
            Model model_to_complain_in_models_tree = Model(model_to_complain); //o(1)
            model_to_complain_in_models_tree.addComplain(months); //o(1)
            this->models_tree.insertElement(model_to_complain_in_models_tree); //o(log(M))
        }
        return SUCCESS;
    };

    StatusType GetBestSellerModelByType(int typeID, int *modelID)
    {
        if(typeID == 0)
        {
            if (this->types_tree.isEmpty())
            {
                return FAILURE;
            }
            int id = best_seller_model.getModelID();
            *modelID = id;
            return SUCCESS; 
        }
        ModelsArray dummy_array = ModelsArray(typeID);
        ModelsArray* type_array = this->types_tree.getNodeData(dummy_array);
        if (type_array == NULL)
        {
            return FAILURE; 
        }
        int id = type_array->getBestSellerModel().getModelID(); 
        *modelID = id;
        return SUCCESS;        
    }


    StatusType GetWorstModels(int numOfModels, int* types, int* models)
    {
        int counter_zero = 0; 
        if(models_tree.getMinNode()->getData().getScore()> 0 )
        {
            counter_zero = goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models, 0, false);
        }
        goThroughModelsTree(models_tree.getMinNode(), numOfModels, types, models, counter_zero, false);
    }

    bool checkForZeroParent(AvlTree<Model>* node)
    {
         if(node->getParent() !=NULL)
            {
                if(node->getData().getScore() < 0 && node->getParent()->getData().getScore() > 0)
                {
                    return true; 
                }
            }
        return false;     
            
    }
    bool checkForZeroRight(AvlTree<Model>* node)
    {
         if(node->getRight() !=NULL)
            {
                if(node->getData().getScore() < 0 && node->getRight()->getData().getScore() > 0)
                {
                    return true; 
                }
            }
        return false;     
            
    }
    int goThroughModelsTree(AvlTree<Model>* starting_node, int numOfModels, int* types, int* models, int i, bool order_flag)
    {
        int counter = 0;
        int counter_zero = i; 
        //in case we went through all nodes in this course. 
        if(starting_node == NULL)
        {
            return i-1; 
        }
        // in case we went through number of models that was asked. 
        if(i == numOfModels)
        {
            return i; // think about that
        }
        // in case we got to the toot. we need to switch to regular inorder. 
        if(starting_node->getParent() ==NULL)
        {
            types[i] = starting_node->getData().getTypeID();
            models[i] = starting_node->getData().getModelID();
            order_flag = true;

            if(checkForZeroRight(starting_node) == true)
            {
                counter_zero = goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models, i, false);
            }   

            goThroughModelsTree(starting_node->getRight(), numOfModels, types, models, counter_zero++, true); 
        }
        // in case we before the root. 
        else if(order_flag == false)
        {
            // do the actual function.
            types[i] = starting_node->getData().getTypeID();
            models[i] = starting_node->getData().getModelID();
            // check if we need to switch to zero tree. 
            if(checkForZeroRight(starting_node) == true)
            {
                counter_zero = goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models, i, false);
            }  

            counter = goThroughModelsTree(starting_node->getRight(), numOfModels, types, models, counter_zero++, false); 
            
            if(checkForZeroParent(starting_node) == true)
            {
                counter_zero = goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models, i, false);
            }
            goThroughModelsTree(starting_node->getParent(), numOfModels, types, models, counter, false);     
        }
        // redular inorder, in case we in the right side of the tree. 
        else
        {
            counter =  goThroughModelsTree(starting_node->getLeft(), numOfModels, types, models, counter_zero++, true); 
            // do the actual function.
            types[i] = starting_node->getData().getTypeID();
            models[i] = starting_node->getData().getModelID();
             // check if we need to switch to zero tree. 
            if(checkForZeroRight(starting_node) == true)
            {
               counter_zero = goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models, i, false);
            }  
            goThroughModelsTree(starting_node->getRight(), numOfModels, types, models, counter, true); 
        }
    }

    int goThroughZeroTree(AvlTree<TypeTree>* starting_node, int numOfModels, int* types, int* models, int i, bool order_flag)
    {
        int counter = 0;
        //in case we went through all nodes in this course. 
        if(starting_node == NULL)
        {
            return i-1; 
        }
        // in case we went through number of models that was asked. 
        if(i == numOfModels)
        {
            return i; // think about that
        }
        
        if(starting_node->getParent() ==NULL)
        {
            goThroughModelsTree(starting_node->getData().getModelsTree().findMinNode(), numOfModels, types, models, i, false); 
            order_flag = true;
            goThroughZeroTree(starting_node->getRight(), numOfModels, types, models, i++, true); 
        }

        else if(order_flag == false)
        {
            // do the actual function.
            goThroughModelsTree(starting_node->getData().getModelsTree().findMinNode(), numOfModels, types, models, i, false);
            counter = goThroughZeroTree(starting_node->getRight(), numOfModels, types, models, i++, false); 
            goThroughZeroTree(starting_node->getParent(), numOfModels, types, models, counter, false);     
        }
        else
        {
            counter = goThroughZeroTree(starting_node->getLeft(), numOfModels, types, models, i++, true); 
            goThroughModelsTree(starting_node->getData().getModelsTree().findMinNode(), numOfModels, types, models, i, false);
            goThroughZeroTree(starting_node->getRight(), numOfModels, types, models, counter, true); 
        }
    }

    // int GetZeroModelsInTypeTree(AvlTree<Model>* min_model, int numOfModels, int save_counter, int* types, int* models)
    // {
    //     Model model;
    //     AvlTree<Model>* right_model;
    //     while (min_model != NULL) // save models in type tree
    //     {
    //         model = min_model->getData(); //o(1)
    //         types[save_counter] = model.getTypeID();
    //         models[save_counter] = model.getModelID();
    //         save_counter++;
    //         if (save_counter == numOfModels)
    //         {
    //             return save_counter;
    //         }
    //         right_model = min_model->getRight();
    //         while (right_model != NULL) // save right branch
    //         {
    //             model = right_model->getData(); //o(1)
    //             types[save_counter] = model.getTypeID();
    //             models[save_counter] = model.getModelID();
    //             save_counter++;
    //             if (save_counter == numOfModels)
    //             {
    //                 return save_counter;
    //             }
    //             right_model = (*right_model).getRight(); //o(1)
    //         }
    //         min_model = (*min_model).getParent(); //o(1)
    //     }
    //     return save_counter;
    // }

    // int GetZeroModels(int numOfModels, int save_counter, int* types, int* models)
    // {
    //     TypeTree type_tree;
    //     Model* model;
    //     AvlTree<TypeTree>* min_tree = this->lowest_zero_model;
    //     AvlTree<TypeTree>* right_tree;
    //     AvlTree<Model>* min_model;
    //     while (min_tree != NULL)
    //     {
    //         type_tree = min_tree->getData(); //o(1)
    //         min_model = type_tree.getLowestModel(); //o(1)
    //         save_counter = GetZeroModelsInTypeTree(min_model, numOfModels, save_counter, types, models);
    //         if (save_counter == numOfModels)
    //         {
    //             return save_counter;
    //         }
    //         right_tree = min_tree->getRight();
    //         while (right_tree != NULL) // print right branch
    //         {
    //             type_tree = right_tree->getData(); //o(1)
    //             min_model = type_tree.getLowestModel(); //o(1)
    //             save_counter = GetZeroModelsInTypeTree(min_model, numOfModels, save_counter, types, models);
    //             if (save_counter == numOfModels)
    //             {
    //                 return save_counter;
    //             }
    //             right_tree = (*right_tree).getRight(); //o(1)
    //         }
    //         min_tree = (*min_tree).getParent(); //o(1)
    //     }
    //     return save_counter;
    // }

    // StatusType GetWorstModels(int numOfModels, int* types, int* models)
    // {
    //     Model model;
    //     int save_counter = 0;
    //     bool is_print_zero = false;
    //     AvlTree<Model>* min_model = this->lowest_model;
    //     AvlTree<Model>* right_model;
    //     while (min_model != NULL)
    //     {
    //         model = min_model->getData(); //o(1)
    //         if (!is_print_zero && model.getScore() > 0) // first time of positive - check zero_tree
    //         {
    //             save_counter = GetZeroModels(numOfModels, save_counter, types, models);
    //             if (save_counter == numOfModels)
    //             {
    //                 return SUCCESS;
    //             }
    //             is_print_zero = true;
    //         }
    //         types[save_counter] = model.getTypeID();
    //         models[save_counter] = model.getModelID();
    //         save_counter++;
    //         if (save_counter == numOfModels)
    //         {
    //             return SUCCESS;
    //         }
    //         right_model = min_model->getRight();
    //         while (right_model != NULL) // print right branch
    //         {
    //             model = right_model->getData(); //o(1)
    //             if (!is_print_zero && model.getScore() > 0) // first time of positive - check zero_tree
    //             {
    //                 save_counter = GetZeroModels(numOfModels, save_counter, types, models);
    //                 if (save_counter == numOfModels)
    //                 {
    //                     return SUCCESS;
    //                 }
    //                 is_print_zero = true;
    //             }
    //             types[save_counter] = model.getTypeID();
    //             models[save_counter] = model.getModelID();
    //             save_counter++;
    //             if (save_counter == numOfModels)
    //             {
    //                 return SUCCESS;
    //             }
    //             right_model = (*right_model).getRight(); //o(1)
    //         }
    //         min_model = (*min_model).getParent(); //o(1)
    //     }
    //     return (save_counter == numOfModels) ? SUCCESS : FAILURE;
    // }
};
#endif