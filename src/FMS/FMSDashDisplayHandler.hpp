#ifndef FMS_DASH_DISPLAY_HANDLER_H
#define FMS_DASH_DISPLAY_HANDLER_H
#include <cstdint>
#include "UtilsFMS.hpp"

#include <stdio.h>
#include <bitset>
class FMSDashDisplayHandler: public FMSHandler {

public:
	FMSDashDisplayHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSDashDisplayHandler() :FMSHandler() {

	}
	;

public:
	~FMSDashDisplayHandler() {
	}
	;

public:
	int calculateFuelLevel() {

		return data[1] * 0.4;
	}
	;

};

#endif
