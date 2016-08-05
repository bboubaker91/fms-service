#ifndef FMS_FUEL_CONSUMPTION_H
#define FMS_FUEL_CONSUMPTION_H
#include <cstdint>
#include "UtilsFMS.hpp"

#include <stdio.h>
#include <bitset>
class FMSFuelConsumptionHandler: public FMSHandler {

public:
	FMSFuelConsumptionHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSFuelConsumptionHandler() :
			FMSHandler() {

	}
	;

public:
	~FMSFuelConsumptionHandler() {
		delete data;
	}
	;


public:
	float calculateEngineTotalFuelUsed() {
		int a;
		memcpy(&a, &data[4], 4 * sizeof(uint8_t));
		return (a * 0.5);
	}
	;

};

#endif
