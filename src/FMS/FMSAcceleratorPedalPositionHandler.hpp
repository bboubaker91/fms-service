#ifndef FMS_ACCELERATOR_PEDAL_POSITION_HANDLER_H
#define FMS_ACCELERATOR_PEDAL_POSITION_HANDLER_H

#include <cstdint>

#include "FMSHandler.hpp"
#include "UtilsFMS.hpp"

class FMSAcceleratorPedalPositionHandler: public FMSHandler {

public:
	FMSAcceleratorPedalPositionHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSAcceleratorPedalPositionHandler() :
			FMSHandler() {
		data = nullptr;

	}
	;
public:
	~FMSAcceleratorPedalPositionHandler() {
	}
	;

public:
	int AcceleratorPedalRatio() {
		return (data[1]*0.4);
	}
	;

public:
	int EngineLoadValue() {
		return (data[2]);
	}
	;

public:
	bool EngineLoadAlert() {
		return (ENGINE_ALERT_LOAD < this->EngineLoadValue());
	}
	;
};

#endif
