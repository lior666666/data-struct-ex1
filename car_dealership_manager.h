#ifndef DS_H_
#define DS_H_
#include "model_by_sale.h"
#include "models_array.h"
#include "avl_tree.h"
#include "library.h"
#include "type_tree.h"
#include <iostream>
#include <stdio.h>

class CarDealershipManager
{
    AvlTree<ModelsArray> types_tree;
    AvlTree<Model> models_tree;
    AvlTree<ModelBySale> models_tree_by_sales;
    AvlTree<TypeTree> zero_tree;
    bool zero_tree_printed; 
    int printed_counter; 
public:
    CarDealershipManager() : zero_tree_printed(false), printed_counter(0) {};

    ~CarDealershipManager()
    {
        CleanZeroTree(zero_tree.getNext());
    }

    void CleanZeroTree(AvlTree<TypeTree>* node)
    {
        if (node != NULL)
        {
            CleanZeroTree(node->getLeft());
            AvlTree<Model>* type_models_tree = node->getData().getModelsTree();
            delete type_models_tree;
            CleanZeroTree(node->getRight());
        }
    }

    StatusType AddCarType(int typeID, int numOfModels)
    {
        // !!!remember to check parameter in library.cpp!!!!
        ModelsArray new_type = ModelsArray(typeID, numOfModels); // maybee a "new" is needed, but I don't think so. // O(m)
        if (types_tree.insertElement(new_type) == false)
        {
            return FAILURE; //already exist typeID
        }
        AvlTree<Model>* models_tree = new AvlTree<Model>(new_type.getModelsArray(), new_type.getNumOfModels()-1); // O(m)  special algoritm for building AVL tree. 

        for (int i = 0; i < numOfModels; i++) //insert to model_by_sale
        {
            ModelBySale model_by_sale = ModelBySale(typeID, i, 0);
            models_tree_by_sales.insertElement(model_by_sale);
        }

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
            type_to_delete_in_zero_tree->getModelsTree()->clearTree(); //o(m)
            delete type_to_delete_in_zero_tree->getModelsTree();
            this->zero_tree.removeElement(*type_to_delete_in_zero_tree); //o(log(n))
        }
        //****************

        //********delete from models_tree and models_tree_by_sales********
        for (int i = 0; i < type_to_delete->getNumOfModels(); i++) //o(m)
        {
            //delete from models_tree
            model_to_delete = type_to_delete->getModelsArray()[i];
            if (model_to_delete.getScore() != 0)
            {
                this->models_tree.removeElement(model_to_delete); //o(log(M))
            }

            //delete from models_tree_by_sales
            ModelBySale dummy_model_by_sale = ModelBySale(typeID, i, model_to_delete.getNumOfSales());
            this->models_tree_by_sales.removeElement(dummy_model_by_sale); //o(log(M))
        }
        //****************

        this->types_tree.removeElement(dummy_models_array); //o(log(n)), delete the type from types_tree
        return SUCCESS;
    };

    StatusType SellCar(int typeID, int modelID)
    {
        ModelsArray dummy_id = ModelsArray(typeID);
        //AvlTree<ModelsArray>* real_Node_id = types_tree.getNode(dummy_id); // log n
        ModelsArray* model_to_change = types_tree.getNodeData(dummy_id);  // log n
        if (model_to_change == NULL)
        {
            return FAILURE;
        }
        int models_array_length = model_to_change->getNumOfModels();
        if (models_array_length <= modelID)
        {
           return FAILURE;
        }
        //ModelsArray model_to_change = real_Node_id->getData(); // log n
        int prev_score = model_to_change->getModelsArray()[modelID].getScore();
        int prev_num_of_sales = model_to_change->getModelsArray()[modelID].getNumOfSales();
        model_to_change->getModelsArray()[modelID].addSale(); // adding 1 sale + chanching total scores. 
        Model updated_model = model_to_change->getModelsArray()[modelID]; // the new model after update of sales. 
        model_to_change->updateBestSellerModel(updated_model); // checks inside the function if it should update the min. 
        
        //update models_tree_by_sales after the sale
        ModelBySale dummy_model_by_sale = ModelBySale(typeID, modelID, prev_num_of_sales);
        this->models_tree_by_sales.removeElement(dummy_model_by_sale); //o(log(M))
        dummy_model_by_sale.addSale();
        this->models_tree_by_sales.insertElement(dummy_model_by_sale); //o(log(M))

        TypeTree dummy_type_tree_id = TypeTree(typeID, 0, NULL); 
        AvlTree<TypeTree>* real_Node_zero_tree_id = zero_tree.getNode(dummy_type_tree_id); // log n
        
        Model dummy_model = Model(typeID, modelID, prev_score);
        bool zero_is_empty = true;
        // means there are models with 0 scores under this ID.
        if(real_Node_zero_tree_id != NULL)  
        { 
            //printf("000000");
            AvlTree<Model>* zero_models_tree = real_Node_zero_tree_id->getData().getModelsTree(); 
            AvlTree<Model>* real_Model_Node = zero_models_tree->getNode(dummy_model); // log m
            if(real_Model_Node != NULL) // if the update model in the type tree
            {
               // printf("1111111");
                real_Node_zero_tree_id->getData().removeModel(dummy_model); // log m
                if(zero_models_tree->isEmpty()) // in case there are no more 0 score models under this ID. 
                {
                 //   printf("2222222");
                    delete zero_models_tree;
                    zero_tree.removeElement(dummy_type_tree_id);
                }
                models_tree.insertElement(updated_model); // log M 
                zero_is_empty = false; 
            }
        }
        // in case zero trees don't have this model.
        if(zero_is_empty == true || real_Node_zero_tree_id == NULL) 
        {
        //    std::cout <<  "sale modelID: " << dummy_model << std::endl;
            if(models_tree.removeElement(dummy_model) == false)//  log M 
            {
                return FAILURE;
            }
            if(updated_model.getScore() == 0) // will add him to zero tree. 
            {
                if(real_Node_zero_tree_id == NULL) // IN CASE THERE Is NO NODE WITH 0 MODELS UNDER THIS ID. 
                {
                //    printf("4444444");
                    AvlTree<Model>* zero_models_tree =  new AvlTree<Model>; 
                    zero_models_tree->insertElement(updated_model);
                    TypeTree new_type_tree_node = TypeTree(typeID, 1, zero_models_tree); 
                    this->zero_tree.insertElement(new_type_tree_node); 
                }
                else // there are models under this ID with zero.
                {
                  //  printf("5555555");
                    real_Node_zero_tree_id->getData().insertModel(updated_model); // log m
                }
            }
            else // we need to insert it back to models tree
            {
             //   printf("6666666");
                models_tree.insertElement(updated_model);
            }
        }
        //models_tree_by_sales.printTree();
        return SUCCESS; 
    }

    StatusType MakeComplaint(int typeID, int modelID, int months)
    {
        ModelsArray dummy_models_array = ModelsArray(typeID); //o(1)
        ModelsArray* type_to_complain = this->types_tree.getNodeData(dummy_models_array); //o(log(n))
        if (type_to_complain == NULL)
        {
      //     printf("1");
            return FAILURE;
        }
        int models_array_length = type_to_complain->getNumOfModels();
        if (models_array_length <= modelID)
        {
      //    printf("2");
            return FAILURE;
        }
        //Model* models_array = type_to_complain->getModelsArray();
        Model old_model_to_complain = type_to_complain->getModelsArray()[modelID];
        type_to_complain->getModelsArray()[modelID].addComplain(months);
        Model new_model_to_complain = type_to_complain->getModelsArray()[modelID];
        int prev_score = old_model_to_complain.getScore();
        int new_score = new_model_to_complain.getScore();
        if (prev_score != 0) //the model in models_tree
        {
            if (new_score != 0) //after the complain - the score is not 0 so the model stay in models_tree
            {
                // std::cout <<  "sale modelID: " << old_model_to_complain << std::endl;
                //  std::cout <<  "sale modelID: " << new_model_to_complain << std::endl;
                // models_tree.printTree();
                if(models_tree.removeElement(old_model_to_complain) == false)//  log M 
                {
           //         printf("3");
                    return FAILURE;
                }
                //Model* update_model = this->models_tree.getNodeData(model_to_complain)->addComplain(months); //o(log(M))
                models_tree.insertElement(new_model_to_complain);
            }
            else //after the complain - the score is 0 so the model goes to zero_tree
            {
                //remove from models_tree
                this->models_tree.removeElement(old_model_to_complain); //o(log(M))

                //add to zero_tree
                TypeTree dummy_tree = TypeTree(typeID, 0, NULL); //o(1)
                TypeTree* type_in_zero_tree = this->zero_tree.getNodeData(dummy_tree); //o(log(n))
                // Model model_to_complain_in_zero_tree = Model(model_to_complain); //o(1)
                // model_to_complain_in_zero_tree.addComplain(months); //o(1)
                if (type_in_zero_tree == NULL) // type id is not in the zero_tree --> this is the first zero score for that type
                {
                    AvlTree<Model>* zero_models_tree =  new AvlTree<Model>; 
                    zero_models_tree->insertElement(new_model_to_complain);
                    TypeTree type_in_zero_tree = TypeTree(typeID, 1, zero_models_tree); 
                    this->zero_tree.insertElement(type_in_zero_tree); 
                }
                else
                {
                    //type_in_zero_tree->insertModel(model_to_complain_in_zero_tree); //o(log(n))
                    type_in_zero_tree->insertModel(new_model_to_complain); //o(log(n))
                }
            }
        }
        else //the model in zero_tree
        {
            TypeTree dummy_tree = TypeTree(typeID, 0, NULL); //o(1)
            AvlTree<TypeTree>* type_in_zero_tree = this->zero_tree.getNode(dummy_tree); // log n
             AvlTree<Model>* zero_models_tree = type_in_zero_tree->getDataPointer()->getModelsTree();
            // std::cout <<  "sale modelID: " << old_model_to_complain << std::endl;
            // type_in_zero_tree->getDataPointer()->getModelsTree()->printTree();
            // std::cout <<  "sale modelID: " << new_model_to_complain << std::endl;
            if (zero_models_tree->removeElement(old_model_to_complain) == false) //o(log(M))
            {
               // printf("4");
                return FAILURE; //for debugging. It should not happen!!
            }
            //type_in_zero_tree->printNode();
            if (zero_models_tree->isEmpty())
            {
               //printf("Testtttttttt");
                delete zero_models_tree;
                this->zero_tree.removeElement(dummy_tree); //o(log(n)
            }
            // Model model_to_complain_in_models_tree = Model(model_to_complain); //o(1)
            // model_to_complain_in_models_tree.addComplain(months); //o(1)
            this->models_tree.insertElement(new_model_to_complain); //o(log(M))
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
            int id = this->models_tree_by_sales.findMaxNode()->getData().getModelID();
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
        this->printed_counter = 0;
        this->zero_tree_printed = false; 
        // models_tree.printTree();
        // zero_tree.printTree();
        if(models_tree.isEmpty())
        {
            goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models);
            this->zero_tree_printed = true; 
        }
        else
        {
            //models_tree.getMinNode()->printNode();
            if(models_tree.getMinNode()->getData().getScore()> 0 )
            {
                goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models);
                this->zero_tree_printed = true; 
            }
           goThroughModelsTree(models_tree.getMinNode(), numOfModels, types, models);
        }
        if(printed_counter<numOfModels)
            return FAILURE;
        return SUCCESS;    
    }

    bool checkForZeroParent(AvlTree<Model>* node)
    {
         if(node->getParent() !=NULL && this->zero_tree_printed == false)
            {
                if(node->getData().getScore() < 0 && node->getParent()->getData().getScore() > 0)
                {
                    return true; 
                }
            }
        return false;     
            
    }
    bool checkForZeroSubTreeRoot(AvlTree<Model>* node, Model sub_tree_root)
    {
         if(node->getLeft()==NULL && this->zero_tree_printed == false)   
         {
             if(node->getData().getScore() > 0 && sub_tree_root.getScore()< 0)
                {
                    return true;
                }
         }
        return false;            
    }
    // bool checkForZeroRight(AvlTree<Model>* node)
    // {
    //      if(node->getRight() !=NULL)
    //         {
    //             if(node->getData().getScore() < 0 && node->getRight()->getData().getScore() > 0)
    //             {
    //                 return true; 
    //             }
    //         }
    //     return false;     
            
    // }
    void goThroughModelsTreeInOrder(AvlTree<Model>* starting_node, int numOfModels, int* types, int* models,  Model sub_tree_root)
    {

        //printf("222222222222222222");
        if(starting_node == NULL)
        {
            return;
            //return counter; 
        }
        // in case we went through number of models that was asked. 
        if(printed_counter >= numOfModels)
        {
            return; // to think about that
        }
        //printf("333333333333333333333333");
       
        //counter =  goThroughModelsTreeInOrder(starting_node->getLeft(), numOfModels, types, models,i ,counter, sub_tree_root); 
        goThroughModelsTreeInOrder(starting_node->getLeft(), numOfModels, types, models, sub_tree_root);
        // do the actual function.
        if(printed_counter >= numOfModels)
        {
            return; // to think about that
        }
        if(checkForZeroSubTreeRoot(starting_node, sub_tree_root))
        {
            //printf("222222222222222222222222222222222222222222");
           //counter = goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models, counter); 
           goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models); 
           this->zero_tree_printed = true; 
        }
        //printf("44444444444444444444444");
        if(printed_counter >= numOfModels)
        {
            return; // to think about that
        }
        types[printed_counter] = starting_node->getData().getTypeID();
        models[printed_counter] = starting_node->getData().getModelID();
        printed_counter++;
        if(printed_counter >= numOfModels)
        {
            return; // to think about that
        }
        // check if we need to switch to zero tree. 
        goThroughModelsTreeInOrder(starting_node->getRight(), numOfModels, types, models, starting_node->getData()); 
        if(checkForZeroParent(starting_node) == true)
        {
            //printf("33333333333333333333333333333333");
            goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models);
            this->zero_tree_printed = true; 
        }  
    }

    
    void goThroughModelsTree(AvlTree<Model>* starting_node, int numOfModels, int* types, int* models)
    {
        
        while(starting_node != NULL && printed_counter<numOfModels)
        {
           // do the actual function.
           //printf("11111111111111111111");
            types[printed_counter] = starting_node->getData().getTypeID();
            models[printed_counter] = starting_node->getData().getModelID();
            printed_counter++;
            // check if we need to switch to zero tree.   
            if(printed_counter>=numOfModels)
                break; 
            //int new_counter
            goThroughModelsTreeInOrder(starting_node->getRight(),  numOfModels, types, models, starting_node->getData());   
           // if (new_counter != 0)
            //    counter =  new_counter; 
            //check if we need to switch to zero tree. 
              if(printed_counter>=numOfModels)
                break; 
            if(checkForZeroParent(starting_node) == true)
            {
                //printf("555555555555555555555555");    
                goThroughZeroTree(zero_tree.getMinNode(),  numOfModels, types, models);
                this->zero_tree_printed = true; 
            } 
            starting_node = starting_node->getParent();    
        }
    }

    void goThroughZeroTree(AvlTree<TypeTree>* starting_node, int numOfModels, int* types, int* models)
    {
        while(starting_node != NULL && printed_counter<numOfModels)
        {
           // do the actual function.
            goThroughModelsTree(starting_node->getData().getModelsTree()->getMinNode(), numOfModels, types, models);
            if(printed_counter>=numOfModels)
                break; 
            goThroughZeroTreeInOrder(starting_node->getRight(),  numOfModels, types, models);
        
           starting_node = starting_node->getParent();    
        }
    }
    void goThroughZeroTreeInOrder (AvlTree<TypeTree>* starting_node, int numOfModels, int* types, int* models)
    {
        if(starting_node == NULL)
        {
            return; 
        }
        // in case we went through number of models that was asked. 
        if(printed_counter>= numOfModels)
        {
            return; 
        }

        goThroughZeroTreeInOrder(starting_node->getLeft(), numOfModels, types, models); 

        // do the actual function.
        goThroughModelsTree(starting_node->getData().getModelsTree()->getMinNode(), numOfModels, types, models);

        goThroughZeroTreeInOrder(starting_node->getRight(), numOfModels, types, models); 
    }
};
#endif