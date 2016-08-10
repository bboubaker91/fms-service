#ifndef CAN_CONFIG_H
#define CAN_CONFIG_H

#include <dbus-c++/connection.h>
#include <string>
#include <actia/platform/can_proxy.h>
#include "GenericCanConfig.hpp"

namespace Lib {
class CanConfig: public Lib::GenericCanConfig {

public:
	CanConfig(DBus::Connection &connection, const std::string &canInterfaceName) :
			Lib::GenericCanConfig::GenericCanConfig(connection,
					"/com/actia/platform/can/" + canInterfaceName) {

	}

public:
	~CanConfig(){};

};
}
#endif
