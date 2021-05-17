  
#ifndef MODELS_ARRAY_H_
#define MODELS_ARRAY_H_
#include "model.h"
#include <iostream>

class ModelsArray
{
    int typeID;
    int num_of_models;
    int max_sales;
    Model best_seller_model;
    Model* array;
public:
    ModelsArray() = default;

    ModelsArray(const ModelsArray& models_array1) = default;

    ~ModelsArray()
    {
        if (this->array != NULL)
        {
            for(int i = 0; i < this->num_of_models; i++)
            {
                delete (&(this->array))[i];
            }
            delete[] &(this->array);
        }
    };

    ModelsArray(int typeID) : typeID(typeID), num_of_models(0), max_sales(0) {}; //dummy

    //check arg before calling the constructor!!!
    ModelsArray(int typeID, int num_of_models) : typeID(typeID), num_of_models(num_of_models), max_sales(0)
    {
        Model* array = new Model[num_of_models];
        for(int i = 0; i < num_of_models; i++)
        {
            array[i] = *(new Model(typeID, i));
        }
        this->best_seller_model = array[0]; // every typeID has at least one model
        this->array = array;
    };

    int getNumOfModels()
    {
        return this->num_of_models;
    };

    Model* getModelsArray()
    {
        return this->array;
    };

    Model getBestSellerModel()
    {
        return this->best_seller_model;
    };

    void updateBestSellerModel(Model new_model)
    {
        if(new_model.getNumOfSales() > best_seller_model.getNumOfSales())
        {
            this->best_seller_model = new_model;
        }
        else if (new_model.getNumOfSales() == best_seller_model.getNumOfSales()) 
        {
            if (new_model.getTypeID() < best_seller_model.getTypeID())
            {
                this->best_seller_model = new_model;
            }
            else if (new_model.getTypeID() == best_seller_model.getTypeID()) 
            {
                if (new_model.getModelID() < best_seller_model.getModelID())
                {
                    this->best_seller_model = new_model;
                }
            }
        }
    }

    friend bool operator==(const ModelsArray& models_array1, const ModelsArray& models_array2);

    friend bool operator<(const ModelsArray& models_array1, const ModelsArray& models_array2);
};

bool operator==(const ModelsArray& models_array1, const ModelsArray& models_array2)
{
    return models_array1.typeID == models_array2.typeID;
}

bool operator<(const ModelsArray& models_array1, const ModelsArray& models_array2)
{
    return models_array1.typeID < models_array2.typeID;
}

bool operator>(const ModelsArray& models_array1, const ModelsArray& models_array2)
{
    return !(models_array1 < models_array2) && !(models_array1 == models_array2);
}
#endif