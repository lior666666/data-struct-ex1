#include "car_dealership_manager.h"

void* Init() {
    CarDealershipManager *DS = new CarDealershipManager();
    return (void*)DS;
}

StatusType AddCarType(void *DS, int typeID, int numOfModels)
{
    return ((CarDealershipManager*)DS)->AddCarType(typeID, numOfModels);
}

StatusType RemoveCarType(void *DS, int typeID)
{
    if (DS == NULL || typeID <= 0)
    {
        return INVALID_INPUT;
    }
    return ((CarDealershipManager*)DS)->RemoveCarType(typeID);
}

StatusType MakeComplaint(void *DS, int typeID, int modelID, int t)
{
    if (DS == NULL || typeID <= 0 || modelID < 0)
    {
        return INVALID_INPUT;
    }
    return ((CarDealershipManager*)DS)->MakeComplaint(typeID, modelID, t);
}

StatusType GetWorstModels(void *DS, int numOfModels, int *types, int *models)
{
    if (DS == NULL || numOfModels <= 0)
    {
        return INVALID_INPUT;
    }
    return ((CarDealershipManager*)DS)->GetWorstModels(numOfModels, types, models);
}

