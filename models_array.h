#ifndef MODELS_ARRAY_H_
#define MODELS_ARRAY_H_
#include "model.h"
#include <iostream>

class ModelsArray
{
    int typeID;
    int num_of_models;
    int max_sales;
    Model* best_seller_model;
    Model* array;
public:
    ModelsArray() = default;
    ModelsArray(const ModelsArray& models_array1) = default;
    ~ModelsArray()
    {
        for(int i = 0; i < this->num_of_models; i++)
        {
            delete (&(this->array))[i];
        }
        delete[] &(this->array);
    };
    //check arg before calling the constructor!!!
    ModelsArray(int typeID, int num_of_models)
    {
        Model* array = new Model[num_of_models];
        for(int i = 0; i < num_of_models; i++)
        {
            array[i] = *(new Model(typeID, i));
        }
        this->typeID = typeID;
        this->num_of_models = num_of_models;
        this->max_sales = 0; // there is no sales when allocation
        this->best_seller_model = &(array[0]); // every typeID has at least one model
        this->array = array;
    };
};
#endif