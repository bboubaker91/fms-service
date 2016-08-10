#ifndef FMS_ACTIVATE_ENGINE_DURATION_HANDLER_H
#define FMS_ACTIVATE_ENGINE_DURATION_HANDLER_H
#include <cstdint>
#include "UtilsFMS.hpp"
#include <stdio.h>
#include <bitset>
class FMSActivateEngineDurationhandler: public FMSHandler {

public:
	FMSActivateEngineDurationhandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;

	FMSActivateEngineDurationhandler() :
			FMSHandler() {

	}
	;

public:
	~FMSActivateEngineDurationhandler() {

	}
	;

public:
	unsigned calculateTotalEngineWorkDuration() {
		uint a;
		memcpy(&a, &data[0], 4 * sizeof(uint8_t));
		return a * 0.05;
	}
	;
};

#endif
