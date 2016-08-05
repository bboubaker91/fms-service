#ifndef FMS_ENGINE_TEMPERATURE_HANDLER_H
#define FMS_ENGINE_TEMPERATURE_HANDLER_H

#include <cstdint>

#include "FMSHandler.hpp"

class FMSEngineLiquideTemperatureHandler: public FMSHandler {

public:
	FMSEngineLiquideTemperatureHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSEngineLiquideTemperatureHandler() :
			FMSHandler() {

	}
	;

public:
	~FMSEngineLiquideTemperatureHandler() {
	}
	;

public:
	uint8_t calculateEngineTemperature() {
		return unsigned (data[0]) -40;
	}
	;

};

#endif
