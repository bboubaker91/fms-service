#ifndef FMS_FUEL_HANDLER_H
#define FMS_FUEL_HANDLER_H
#include <cstdint>
#include "UtilsFMS.hpp"

#include <stdio.h>
#include <bitset>
class FMSFuelHandler: public FMSHandler {

public:
	FMSFuelHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSFuelHandler() :
			FMSHandler() {

	}
	;

public:
	~FMSFuelHandler() {
	}
	;

public:
	float calculateFuelRate() {
		uint16_t a;
		memcpy(&a, &data[0], 2 * sizeof(uint8_t));
		return a * 0.05;
	};

public:
	float calculateFuelEconomy() {
		uint16_t a;
		memcpy(&a, &data[2], 2 * sizeof(uint8_t));
		return a / 512;
	}
	;

};

#endif
