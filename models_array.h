#ifndef MODELS_ARRAY_H_
#define MODELS_ARRAY_H_
#include "model.h"
#include <iostream>

class ModelsArray
{
    int typeID;
    int num_of_models;
    Model best_seller_model;
    Model* array;
public:
    ModelsArray(): typeID(0), num_of_models(0), best_seller_model(), array() {}

    ModelsArray(const ModelsArray& models_array1) :
    typeID(models_array1.typeID), num_of_models(models_array1.num_of_models), best_seller_model(models_array1.best_seller_model), array()
    {
        Model* array = new Model[num_of_models];
        for(int i = 0; i < num_of_models; i++)
        {
            array[i] = Model(models_array1.array[i]);
        }
        this->array = array;
    }

    ModelsArray& operator=(const ModelsArray& models_array1)
    {
        if (this == &models_array1)
        {
		    return *this;
        }
        delete[] this->array;
        this->array = new Model[models_array1.num_of_models];
        this->num_of_models = models_array1.num_of_models;
        this->typeID = models_array1.typeID;
        this->best_seller_model = models_array1.best_seller_model;
        for(int i = 0; i < num_of_models; i++)
        {
            array[i] = models_array1.array[i];
        }
        return *this;
    }

    ~ModelsArray()
    {
        if (this->array != NULL)
        {
            delete[] this->array;
        }
    }

    ModelsArray(int typeID) : typeID(typeID), num_of_models(0), best_seller_model(), array() {} //dummy

    //check arg before calling the constructor!!!
    ModelsArray(int typeID, int num_of_models) : typeID(typeID), num_of_models(num_of_models), best_seller_model(), array()
    {
        Model* array = new Model[num_of_models];
        for(int i = 0; i < num_of_models; i++)
        {
            array[i] = Model(typeID, i, 0);
        }
        this->best_seller_model = array[0]; // every typeID has at least one model
        this->array = array;
    }

    int getTypeID()
    {
        return this->typeID;
    }

    int getNumOfModels()
    {
        return this->num_of_models;
    }

    Model* getModelsArray()
    {
        return this->array;
    }

    Model getBestSellerModel()
    {
        return this->best_seller_model;
    }

    bool updateBestSellerModel(Model new_model)
    {
        if(new_model.getNumOfSales() > best_seller_model.getNumOfSales())
        {
            this->best_seller_model = new_model;
            return true;
        }
        else if (new_model.getNumOfSales() == best_seller_model.getNumOfSales()) 
        {
            if (new_model.getTypeID() < best_seller_model.getTypeID())
            {
                this->best_seller_model = new_model;
                return true;
            }
            else if (new_model.getTypeID() == best_seller_model.getTypeID()) 
            {
                if (new_model.getModelID() < best_seller_model.getModelID())
                {
                    this->best_seller_model = new_model;
                    return true;
                }
            }
        }
        return false;
    }

    friend bool operator==(const ModelsArray& models_array1, const ModelsArray& models_array2);

    friend bool operator<(const ModelsArray& models_array1, const ModelsArray& models_array2);

    friend std::ostream& operator<<(std::ostream& os, const ModelsArray& c);
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

std::ostream& operator<<(std::ostream& os, const ModelsArray& c)
{
    return os << "modelArray:" << c.typeID << ":" << c.num_of_models << "***";
}
#endif