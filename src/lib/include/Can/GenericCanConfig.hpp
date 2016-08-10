#ifndef GENERIC_CAN_CONFIG_H
#define GENERIC_CAN_CONFIG_H
#include <string>
#include <actia/platform/can_proxy.h>
#include <dbus-c++/connection.h>
#include <dbus-c++/object.h>
#include "CanTypeDefs.h"

namespace Lib {
class GenericCanConfig: public DBus::ObjectProxy,
		public com::actia::platform::can::config_proxy {
public:
	GenericCanConfig(DBus::Connection &connection,
			const std::string &genericPath) :
			DBus::ObjectProxy(connection, genericPath, "com.actia.platform.can") {

	}


	~GenericCanConfig(){};


};
}
#endif
