#ifndef FMS_ENGINE_HANDLER_H
#define FMS_ENGINE_HANDLER_H
#include <cstdint>
#include "UtilsFMS.hpp"

#include <stdio.h>
#include <bitset>
class FMSEngineHandler: public FMSHandler {

public:
	FMSEngineHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSEngineHandler() :
			FMSHandler() {

	}
	;

public:
	~FMSEngineHandler() {
		delete data;
	}
	;


public:
	uint8_t calculateActualPercentTorque() {
		return data[2]+125;
	}
	;
public:
	float calculateEngineSpeed() {
		uint16_t a;
		memcpy(&a, &data[3], 2 * sizeof(uint8_t));
		return (unsigned (a) * 0.125);
	}
	;

};

#endif
