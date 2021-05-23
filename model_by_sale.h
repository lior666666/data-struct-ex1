#ifndef MODEL_BY_SALE_H_
#define MODEL_BY_SALE_H_
#include <iostream>

class ModelBySale
{
    int typeID;
    int modelID;
    int num_of_sales;
public:

    ModelBySale(): typeID(0), modelID(0), num_of_sales(0) {}
    ~ModelBySale() = default;

    ModelBySale(const ModelBySale& model) = default;

    friend bool operator==(const ModelBySale& model1, const ModelBySale& model2);

    friend bool operator<(const ModelBySale& model1, const ModelBySale& model2);

    ModelBySale(int typeID, int modelID, int num_of_sales) : typeID(typeID), modelID(modelID), num_of_sales(num_of_sales) {}
    
    void addSale()
    {
        this->num_of_sales = this->num_of_sales + 1;
    }

    int getTypeID()
    {
        return this->typeID;
    }

    int getModelID()
    {
        return this->modelID;
    }

    int getNumOfSales()
    {
        return this->num_of_sales;
    }
};

bool operator==(const ModelBySale& model1, const ModelBySale& model2)
{
    return model1.num_of_sales == model2.num_of_sales &&
            model1.typeID == model2.typeID &&
            model1.modelID == model2.modelID;
}

bool operator<(const ModelBySale& model1, const ModelBySale& model2)
{
    if(model1.num_of_sales == model2.num_of_sales)
    {
        if(model1.typeID == model2.typeID)
        {
            return model1.modelID > model2.modelID;
        }
        return model1.typeID > model2.typeID;
    }
    return model1.num_of_sales < model2.num_of_sales;
}

bool operator>(const ModelBySale& model1, const ModelBySale& model2)
{
    return !(model1 < model2) && !(model1 == model2);
}

#endif
