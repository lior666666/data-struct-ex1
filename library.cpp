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


