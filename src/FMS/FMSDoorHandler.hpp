#ifndef FMS_DOOR_HANDLER_H
#define FMS_DOOR_HANDLER_H
#include <cstdint>
#include "UtilsFMS.hpp"

#include <stdio.h>
#include <bitset>
class FMSDoorHandler: public FMSHandler {

public:
	FMSDoorHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSDoorHandler() :
			FMSHandler() {

	}
	;

public:
	~FMSDoorHandler() {
		delete data;

	}
	;

public:
	bool checkErrorDoorsState() {
		std::bitset<4> tmp(data[0]);
		return (tmp == 0b1110);
	}
	;
public:
	bool checkNotAvailableDoorsState() {
		std::bitset<4> tmp(data[0]);
		return (tmp == 0b1111);

	}
	;

public:
	bool calculateAllClosedDoorsStatus() {
		std::bitset<4> tmp(data[0]);
		return (tmp == 0b0010);

	}
	;

public:
	bool calculateAtLeastOneDoorOpenStatus() {
		std::bitset<4> tmp(data[0]);
		return (tmp == 0b0000);
	}	;

};

#endif
