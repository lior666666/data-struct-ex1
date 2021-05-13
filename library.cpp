#include "car_dealership_manager.h"

void* Init() {
    CarDealershipManager *DS = new CarDealershipManager();
    return (void*)DS;
}

StatusType AddCarType(void *DS, int typeID, int numOfModels)
{
    if (DS == NULL || typeID <= 0 || numOfModels <= 0)
    {
        return INVALID_INPUT;
    }
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

StatusType SellCar(void *DS, int typeID, int modelID)
{
    if (DS == NULL || typeID <= 0 || modelID < 0)
    {
        return INVALID_INPUT;
    }
    return ((CarDealershipManager*)DS)->SellCar(typeID, modelID);
}

StatusType MakeComplaint(void *DS, int typeID, int modelID, int t)
{
    if (DS == NULL || typeID <= 0 || modelID < 0)
    {
        return INVALID_INPUT;
    }
    return ((CarDealershipManager*)DS)->MakeComplaint(typeID, modelID, t);
}

StatusType GetBestSellerModelByType(void *DS, int typeID, int * modelID)
{
    if (DS == NULL || typeID < 0)
    {
        return INVALID_INPUT;
    }
    return ((CarDealershipManager*)DS)->GetBestSellerModelByType(typeID, modelID);
}

StatusType GetWorstModels(void *DS, int numOfModels, int *types, int *models)
{
    if (DS == NULL || numOfModels <= 0)
    {
        return INVALID_INPUT;
    }
    return ((CarDealershipManager*)DS)->GetWorstModels(numOfModels, types, models);
}

