#ifndef CAN__H
#define CAN__H



#include <dbus-c++/connection.h>
#include <dbus-c++/dispatcher.h>
#include <dbus-c++/glib-integration.h>
#include <string>
#include "CanConfig.hpp"
#include "CanStatus.hpp"
#include "CanTypeDefs.h"
using namespace Lib;

class Can {
public:
	Can(DBus::Connection &bus, const std::string &canInterfaceName) {
		configuration = new CanConfig(bus, canInterfaceName);
		status = new CanStatus(bus, canInterfaceName);
	}
	Can(DBus::Connection &bus, enum CanInterface canInterface) {
		switch (canInterface)

		{
		case CanInterface::can0:
			configuration = new CanConfig(bus, "can0");
			status = new CanStatus(bus, "can0");
			break;
		case CanInterface::can1:
			configuration = new CanConfig(bus, "can1");
			status = new CanStatus(bus, "can1");
		}
	}
	virtual ~Can() {
		delete configuration;
		delete status;
	}
	CanConfig* getConfiguration() {
		return this->configuration;
	}
	CanStatus* getStatus()
	{
		return this->status;
	}
private:

	CanConfig* configuration;
	CanStatus* status;
};
#endif
