#ifndef MODEL_H_
#define MODEL_H_
#include <iostream>

class Model
{
    int typeID;
    int modelID;
    int score;
    int num_of_sales;
    int num_of_complaints;
public:
    Model() = default;
    ~Model() = default;
    Model(const Model& model) = default;
    friend bool operator==(const Model& model1, const Model& model2);
    friend bool operator<(const Model& model1, const Model& model2);
    friend std::ostream& operator<<(std::ostream& os, const Model& c);
    Model(int typeID, int modelID, int score)
    : typeID(typeID), modelID(modelID), score(score)
    {
        this->num_of_sales = 0;
        this->num_of_complaints = 0;
    }
    void addSale()
    {
        this->num_of_sales = this->num_of_sales + 1;
        this->score = (this->score)+10;
    }
    Model* addComplain(int months)
    {
        this->num_of_complaints = this->num_of_complaints + 1;
        this->score = this->score-(100/months);
        return this;
    }
    int getScore()
    {
        return this->score;
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

bool operator==(const Model& model1, const Model& model2)
{
    return model1.score == model2.score &&
            model1.typeID == model2.typeID &&
            model1.modelID == model2.modelID;
}

bool operator<(const Model& model1, const Model& model2)
{
    if(model1.score == model2.score)
    {
        if(model1.typeID == model2.typeID)
        {
            return model1.modelID < model2.modelID;
        }
        return model1.typeID < model2.typeID;
    }
    return model1.score < model2.score;
}

bool operator>(const Model& model1, const Model& model2)
{
    return !(model1 < model2) && !(model1 == model2);
}

std::ostream& operator<<(std::ostream& os, const Model& c)
{
	return os << "model: typeID: " << c.typeID << ", modelID: " << c.modelID << ", score: " << c.score << "***";
}
#endif