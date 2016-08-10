#ifndef FMS_GEAR_HANDLER_H
#define FMS_GEAR_HANDLER_H

#include <cstdint>

#include "FMSHandler.hpp"

class FMSGearhandler: public FMSHandler {

public:
	FMSGearhandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSGearhandler() :
			FMSHandler() {

	}
	;

public:
	~FMSGearhandler() {

	}
	;

public:
	int calculateCurrentGear() {
		return this->data[3] - 125;
	}
	;

public:
	int calculateSelectedGear() {
		return this->data[0] - 125;
	}
	;



};

#endif
