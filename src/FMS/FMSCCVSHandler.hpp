#ifndef FMS_WHEEL_SPEED_HANDLER_H
#define FMS_WHEEL_SPEED_HANDLER_H

#include <sys/types.h>
#include <cstdint>
#include <cstring>

#include "FMSHandler.hpp"

enum BrakeState {
	PRESSED, NOT_PRESSED, UNKNOWN_BRAKE_STATE
};

enum ParkingBrakeState {
	SET, NOT_SET, UNKNOWN_PARKING_BRAKE_STATE
};

class FMSCCVSHandler: public FMSHandler {

public:
	FMSCCVSHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSCCVSHandler() :
			FMSHandler() {

	}
	;

public:
	~FMSCCVSHandler() {
	}
	;

public:
	void setData(uint8_t *data) {
		this->data = data;
	}
	;

public:
	uint8_t* getData() {
		return this->data;
	}
	;

public:
	float calculateWheelSpeed() {
		uint16_t a;
		memcpy(&a, &data[1], 2 * sizeof(uint8_t));
		return (unsigned (a) / 256);
	}
	;

public:

	enum BrakeState getBrakeSwitchState() {
		std::bitset<8> tmp(data[3]);
		if ((tmp[5] == 0) && (tmp[4] == 0)) {
			return BrakeState::NOT_PRESSED;

		}
		if ((tmp[5] == 0) && (tmp[4] == 1)) {
			return BrakeState::PRESSED;

		}

		return BrakeState::UNKNOWN_BRAKE_STATE;
	}
	;

public:

	enum ParkingBrakeState getParkingBrakeSwitchState() {
		std::bitset<8> tmp(data[0]);
		if ((tmp[3] == 0) && (tmp[2] == 0)) {

			return ParkingBrakeState::NOT_SET;
		}
		if ((tmp[3] == 0) && (tmp[2] == 1)) {
			return ParkingBrakeState::SET;
		}
		return ParkingBrakeState::UNKNOWN_PARKING_BRAKE_STATE;
	}
	;

};

#endif
