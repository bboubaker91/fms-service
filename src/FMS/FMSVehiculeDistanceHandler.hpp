#ifndef FMS_VEHICULE_DISTANCE_HANDLER_H
#define FMS_VEHICULE_DISTANCE_HANDLER_H

#include <string.h>
#include <sys/types.h>
#include <cstdint>

#include "FMSHandler.hpp"

class FMSVehiculeDistanceHandler: public FMSHandler {

public:
	FMSVehiculeDistanceHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {

	};
public:
	FMSVehiculeDistanceHandler() :
			FMSHandler() {

	}
	;

public:
	~FMSVehiculeDistanceHandler() {

	}
	;


public:
	unsigned calculateTotalDistance() {
		uint32_t a;
		memcpy(&a, &data[0], 4 * sizeof(uint8_t));
		return (unsigned (a) * 5);
	};

};

#endif
