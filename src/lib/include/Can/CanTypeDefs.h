#ifndef CAN_TYPE_DEFS_H
#define CAN_TYPE_DEFS_H
#include <string>
//std::string  serviceStr="com.actia.platform.can";
//std::string configInterfaceStr ="com.actia.platform.can.config";
//std::string configGenericpathStr="/com/actia/platform/can/";



//std::string StatusInterfaceStr="com.actia.platform.can.status";
//std::string StatusconfigInterfaceStr="/com/actia/platform/can/";


enum FilterType {
	/// @brief Receive filter is based only on CAN id
	FILTER_ID,
	/// @brief Receive filter is based on changes in dlc; if dlc
	///       changes a message is received
	FILTER_DLC,
	/// @brief Filter is based on changes in data; if any data in
	///       message changes a message is received
	FILTER_DATA,
};

enum CanState {
	ErrorActive, ErrorWarning, ErrorPassive, BusOff, Stopped, Sleeping, Unknown,
};

enum CanInterface {
	/// @set interface for can0
	can0,
	/// @set interface for can0
	can1,
};

#endif
