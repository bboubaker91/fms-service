/** Copyright (C) 2015 ACTIA NORDIC AB. All rights reserved. **/
#ifndef SYSTEMFIREWALL_H
#define SYSTEMFIREWALL_H

#include <actia/platform/system_proxy.h>

#include <vector>
#include <string>
#include <map>

class SystemFirewall:
    public DBus::ObjectProxy,
    public com::actia::platform::system::firewall_proxy {
 public:
  SystemFirewall(DBus::Connection &connection):DBus::ObjectProxy(connection,
          "/com/actia/platform/system/firewall",
          "com.actia.platform.system") {
};
  ~SystemFirewall()
  {

  };
};

#endif
