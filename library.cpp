#include "library.h"
#include "car_dealership_manager.h"

void* Init() {
    CarDealershipManager *DS = new CarDealershipManager();
    return (void*)DS;
}

StatusType AddCarType(void *DS, int typeID, int numOfModels) {
    return ((CarDealershipManager*)DS)->AddCarType(typeID, numOfModels);
}


