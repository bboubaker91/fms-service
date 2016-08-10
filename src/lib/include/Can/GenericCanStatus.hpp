#ifndef GENERIC_CAN_STATUS_H
#define GENERIC_CAN_STATUS_H

#include <actia/platform/can_proxy.h>
#include <dbus-c++/connection.h>
#include <dbus-c++/object.h>
#include <string>

#include <actia/platform/can_proxy.h>
#include <dbus-c++/connection.h>
#include <dbus-c++/object.h>
#include <string>
namespace Lib {
class GenericCanStatus: public DBus::ObjectProxy,
		public com::actia::platform::can::status_proxy {
public:

	GenericCanStatus(DBus::Connection &connection,
			const std::string &genericPath) :
			DBus::ObjectProxy(connection, genericPath, "com.actia.platform.can") {
	};
	~GenericCanStatus() {
	};

};
}
#endif
