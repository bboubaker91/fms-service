#ifndef FMS_AMB_HANDLER_H
#define FMS_AMB_HANDLER_H
#include <cstdint>
#include "UtilsFMS.hpp"
#include <stdio.h>
#include <bitset>
class FMSAMBHandler: public FMSHandler {

public:
	FMSAMBHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	}
	;
public:
	FMSAMBHandler() :
			FMSHandler() {

	}
	;

public:
	~FMSAMBHandler() {
		delete data;
	}
	;


public:
	int calculateAMB() {
		int a;
		memcpy(&a, &data[3], 2 * sizeof(uint8_t));
		return (a * 0.03125) + 273;
	}
	;

};

#endif
