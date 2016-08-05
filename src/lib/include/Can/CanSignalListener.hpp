#ifndef CAN_PROPERTIES_LISTENER_H
#define CAN_PROPERTIES_LISTENER_H
#include <dbus-c++/property.h>
#include <dbus-c++/types.h>
#include <map>
#include <string>
#include <vector>
//#include "CanSignalListener.h"

#include "CanConfig.hpp"


class CanSignalListener: public DBus::PropertiesProxy {
public:
	virtual void SignalReceived(const std::string &interface_name,
			const std::map<std::string, ::DBus::Variant> &changed_properties,
			const std::vector<std::string> &invalidated_properties)=0;

};
#endif
