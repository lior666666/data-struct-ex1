#include "car_dealership_manager.h"

void* Init()
{
    CarDealershipManager* DS = NULL;
	try {
        DS = new CarDealershipManager();
    }
    catch (std::bad_alloc& e) {
        return NULL;
    }
    return (void*)DS;
}

void Quit(void** DS)
{
    delete DS;
}

StatusType AddCarType(void *DS, int typeID, int numOfModels)
{
    if (DS == NULL || typeID <= 0 || numOfModels <= 0)
    {
        return INVALID_INPUT;
    }
    StatusType status;
	try {
        status = ((CarDealershipManager*)DS)->AddCarType(typeID, numOfModels);
    }
    catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
    return status;
}

StatusType RemoveCarType(void *DS, int typeID)
{
    if (DS == NULL || typeID <= 0)
    {
        return INVALID_INPUT;
    }
    StatusType status;
	try {
        status = ((CarDealershipManager*)DS)->RemoveCarType(typeID);
    }
    catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
    return status;
}

StatusType SellCar(void *DS, int typeID, int modelID)
{
    if (DS == NULL || typeID <= 0 || modelID < 0)
    {
        return INVALID_INPUT;
    }
    StatusType status;
	try {
        status = ((CarDealershipManager*)DS)->SellCar(typeID, modelID);
    }
    catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
    return status;
}

StatusType MakeComplaint(void *DS, int typeID, int modelID, int t)
{
    if (DS == NULL || typeID <= 0 || modelID < 0)
    {
        return INVALID_INPUT;
    }
    StatusType status;
	try {
        status = ((CarDealershipManager*)DS)->MakeComplaint(typeID, modelID, t);
    }
    catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
    return status;
}

StatusType GetBestSellerModelByType(void *DS, int typeID, int * modelID)
{
    if (DS == NULL || typeID < 0)
    {
        return INVALID_INPUT;
    }
    StatusType status;
	try {
        status = ((CarDealershipManager*)DS)->GetBestSellerModelByType(typeID, modelID);
    }
    catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
    return status;
}

StatusType GetWorstModels(void *DS, int numOfModels, int *types, int *models)
{
    if (DS == NULL || numOfModels <= 0)
    {
        return INVALID_INPUT;
    }
    StatusType status;
	try {
        status = ((CarDealershipManager*)DS)->GetWorstModels(numOfModels, types, models);
    }
    catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
    return status;
}

