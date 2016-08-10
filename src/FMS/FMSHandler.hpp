#ifndef FMS_HANDLER_H
#define FMS_HANDLER_H
#include <cstdint>
#include "UtilsFMS.hpp"

#include <stdio.h>
#include <bitset>

class FMSHandler {

protected:
	uint32_t id;
protected:
	uint8_t *data;

protected:
	uint8_t dlc;

public:
	FMSHandler(uint32_t id, uint8_t data[], uint8_t dlc) {
		this->id = id;
		this->data = data;
		this->dlc = dlc;
	}
	;
public:
	FMSHandler() {
		data = nullptr;
		id = FMS_ID_NOT_DEFINED;
		dlc = FMS_DLC_NOT_DEFINED;
	}
	;

public:
	virtual ~FMSHandler() {
	}
	;

public:
	void setDATA(uint8_t *data) {
		this->data = data;
	}
	;

public:
	void setID(uint32_t id) {
		this->id = id;
	}
	;
public:
	void setDLC(uint8_t dlc) {
		this->dlc = dlc;
	}
	;

public:
	uint8_t* getDATA() {
		return this->data;
	}
	;

public:
	uint32_t getID() {
		return this->id;
	}
	;
public:
	uint8_t getDLC() {
		return this->dlc;
	}
	;

};

#endif
