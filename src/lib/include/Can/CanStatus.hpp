#ifndef CAN_STATUS_H
#define CAN_STATUS_H
#include <iostream>
#include <dbus-c++/connection.h>
#include <string>

#include "GenericCanStatus.hpp"

namespace Lib {
class CanStatus: public Lib::GenericCanStatus {
public:
	CanStatus(DBus::Connection &connection, const std::string &canInterfaceName) :
			Lib::GenericCanStatus::GenericCanStatus(connection,
					"/com/actia/platform/can/" + canInterfaceName) {
	}
	;
public:
	~CanStatus() {
	}
	;

};
}
#endif
