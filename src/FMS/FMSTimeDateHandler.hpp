#ifndef FMS_DATA_HANDLER_H
#define FMS_DATA_HANDLER_H
#include <cstdint>
#include "UtilsFMS.hpp"

#include "FMSHandler.hpp"

class FMSTimeDateHandler: public FMSHandler {

public:
	FMSTimeDateHandler(uint32_t id, uint8_t data[], uint8_t dlc) :
			FMSHandler(id, data, dlc) {
	}
	;
public:
	FMSTimeDateHandler() :
			FMSHandler() {
	}
	;
public:
	virtual ~FMSTimeDateHandler() {
	}
	;
public:
	int getTimeSecond() {
		return 0.25 * data[0];
	}
	;
public:
	int getTimeMinute() {
		return data[1];
	}
	;

public:
	int getDateHours() {
		return data[2];
	}
	;

public:
	int getDatemonth() {
		return data[3];

	}
	;

public:
	int getDateDay() {
		return data[4] * 0.25;
	}
	;
public:
	int getDateYear() {
		return data[5] + 1985;
	}
	;

public:
	std::string getTimeFullFormatString() {

		std::string str;

		std::stringstream ss;


		ss << this->getDateDay();
		str = ss.str();
		ss.str(std::string());
		ss << this->getDatemonth();
		str += "-" + ss.str();
		ss.str(std::string());
		ss << this->getDateYear();
		str += "-" + ss.str();
		ss.str(std::string());
		ss << this->getDateHours();
		str += "T" + ss.str();
		ss.str(std::string());
		ss << this->getTimeMinute();
		str += ":" + ss.str();
		ss.str(std::string());
		ss << this->getTimeSecond();
		str += ":" + ss.str();
		return str;
	}

};
#endif
