/*
 * Copyright (C) Your copyright notice.
 *
 * Author: BOUBAKER Bassem
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the PG_ORGANIZATION nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY	THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS-IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <boost/test/tools/old/interface.hpp>
#include <dbus-c++/connection.h>
#include <dbus-c++/property.h>
#include <dbus-c++/types.h>
#include <dbus-c++/dispatcher.h>
#include <dbus-c++/glib-integration.h>
#include <stddef.h>
#include "CanConfig.hpp"
#include <iostream>
#include "SocketCan.h"
#include <arpa/inet.h>

#include "BroadcastManager.hpp"
#include "Can.hpp"
#include "CanSignalListener.hpp"
using namespace std;

#define BOOST_TEST_MODULE CAN_LIB_TEST

#include <boost/test/included/unit_test_framework.hpp>

/*class TestPropertiesListener: public DBus::PropertiesProxy,
 public Lib::CanConfig {
 public:
 TestPropertiesListener(DBus::Connection &connection,
 const std::string &canInterfaceName) :
 Lib::CanConfig(connection, canInterfaceName) {
 //connect_signal("com.actia.platform.can",PropertiesChangedcb,PropertiesChanged);
 //org.freedesktop.DBus
 connection.add_match("type='signal',sender='com.actia.platform.can'");

 }

 ~TestPropertiesListener() {

 }
 virtual void PropertiesChanged(const std::string &interface_name,
 const std::map<std::string, ::DBus::Variant> &changed_properties,
 const std::vector<std::string> &invalidated_properties) {
 for (auto p : changed_properties) {
 std::cout << "Property updated : " << p.first << std::endl;
 BOOST_TEST_MESSAGE("properties changed");
 }
 }
 };*/

BOOST_AUTO_TEST_CASE( Test_Dbus_session_Deamon ) {

	BOOST_TEST_MESSAGE("...System DBus connection testing...");
	// Specify the default dispatcher for D-Bus
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	BOOST_REQUIRE(bus.connected() == true);
	BOOST_TEST_MESSAGE("Test passed!");
}

BOOST_AUTO_TEST_CASE( Test_Dbus_system_Deamon ) {

	BOOST_TEST_MESSAGE("...Session DBus connection testing...");
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SystemBus();
	BOOST_REQUIRE(bus.connected() == true);
	BOOST_TEST_MESSAGE("Test passed!");
}

BOOST_AUTO_TEST_CASE( Test_CAN0 ) {

	BOOST_TEST_MESSAGE("...Test CAN0 Interface configuration...");
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	BOOST_REQUIRE(bus.connected() == true);
	Lib::CanConfig can0(bus, "can0");
	can0.StartInterface(250000);
	BOOST_REQUIRE(can0.LinkState() == true);
	can0.StopInterface();
	BOOST_REQUIRE(can0.LinkState() == false);

	BOOST_TEST_MESSAGE("Test passed!");
}

BOOST_AUTO_TEST_CASE( Test_CAN1 ) {

	BOOST_TEST_MESSAGE("...Test CAN1 Interface configuration...");
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	BOOST_REQUIRE(bus.connected() == true);
	Lib::CanConfig can1(bus, "can1");
	can1.StartInterface(250000);
	BOOST_REQUIRE(can1.LinkState() == true);
	can1.StopInterface();
	BOOST_REQUIRE(can1.LinkState() == false);

	BOOST_TEST_MESSAGE("Test passed!");
}

BOOST_AUTO_TEST_CASE( Test_CAN0_status ) {

	BOOST_TEST_MESSAGE("...Test CAN0 Interface status...");
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	BOOST_REQUIRE(bus.connected() == true);
	Lib::CanStatus can0(bus, "can0");
	BOOST_REQUIRE(can0.BusState() != 5);
	BOOST_TEST_MESSAGE("Test passed!");
}

BOOST_AUTO_TEST_CASE( Test_CAN1_status ) {

	BOOST_TEST_MESSAGE("...Test CAN1 Interface status...");
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	BOOST_REQUIRE(bus.connected() == true);
	Lib::CanStatus can1(bus, "can1");
	BOOST_REQUIRE(can1.BusState() != 5);

	BOOST_TEST_MESSAGE("Test passed!");
}

BOOST_AUTO_TEST_CASE( Test_CAN0_configuration_status ) {

	BOOST_TEST_MESSAGE("...Test CAN0 Interface configuration and status...");
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	BOOST_REQUIRE(bus.connected() == true);
	Lib::CanStatus can0Status(bus, "can0");
	BOOST_REQUIRE(can0Status.BusState() != 5);
	Lib::CanConfig can0Config(bus, "can0");
	can0Config.StartInterface(250000);
	BOOST_REQUIRE(can0Config.LinkState() == true);
	can0Config.StopInterface();
	BOOST_REQUIRE(can0Config.LinkState() == false);
	BOOST_TEST_MESSAGE("Test passed!");
}

BOOST_AUTO_TEST_CASE( Test_CAN1_configuration_status ) {

	BOOST_TEST_MESSAGE("...Test CAN1 Interface configuration and status...");
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	BOOST_REQUIRE(bus.connected() == true);
	Lib::CanStatus can1Status(bus, "can1");
	BOOST_REQUIRE(can1Status.BusState() != 5);
	Lib::CanConfig can1Config(bus, "can1");
	can1Config.StartInterface(250000);
	BOOST_REQUIRE(can1Config.LinkState() == true);
	can1Config.StopInterface();
	BOOST_REQUIRE(can1Config.LinkState() == false);
	BOOST_TEST_MESSAGE("Test passed!");
}
BOOST_AUTO_TEST_CASE( Test_Stop_CAN ) {

	BOOST_TEST_MESSAGE("...Test CAN1 Interface configuration and status...");

}

BOOST_AUTO_TEST_CASE( Test_Lib_CAN0 ) {

	BOOST_TEST_MESSAGE("...Test CAN0 Lib...");
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	Can can0(bus, "can0");
	can0.getConfiguration()->StartInterface(250000);
	BOOST_REQUIRE(can0.getConfiguration()->LinkState() == true);
	can0.getConfiguration()->StopInterface();
	BOOST_REQUIRE(can0.getConfiguration()->LinkState() == false);
	BOOST_TEST_MESSAGE("Test passed!");
}

BOOST_AUTO_TEST_CASE( Test_Lib_CAN1 ) {
	BOOST_TEST_MESSAGE("...Test CAN1 Lib...");
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	Can can1(bus, "can1");
	can1.getConfiguration()->StartInterface(250000);
	BOOST_REQUIRE(can1.getConfiguration()->LinkState() == true);
	can1.getConfiguration()->StopInterface();
	BOOST_REQUIRE(can1.getConfiguration()->LinkState() == false);
	BOOST_TEST_MESSAGE("Test passed!");
}

BOOST_AUTO_TEST_CASE( Test_Lib_Broadcast_Manager_sending_mechanism ) {
	BOOST_TEST_MESSAGE(
			"...Test Broadcast Manager for can0 sending mechanism...");
	uint16_t id = 0x0FD;
	uint8_t dlc = 8;
	uint8_t data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	Can can0(bus, CanInterface::can0);
	can0.getConfiguration()->StartInterface(250000);
	BOOST_REQUIRE(can0.getConfiguration()->LinkState() == true);
	BroadcastManager bcm;
	bcm.open(CanInterface::can0);
	BOOST_REQUIRE(bcm.send_periodic(id, data, dlc, 100) == true);
	sleep(1);
	BOOST_REQUIRE(bcm.cancel_send_periodic(id) == true);
	sleep(1);
	BOOST_REQUIRE(bcm.send(id, data, dlc) == true);
	sleep(1);
	BOOST_TEST_MESSAGE("Test passed!");
}

/*
 BOOST_AUTO_TEST_CASE( Test_Lib_Broadcast_Manager_receiving_mechanism ) {
 BOOST_TEST_MESSAGE(
 "...Test Broadcast Manager for can0 receiving mechanism...");
 auto loop = g_main_loop_new(NULL, false);
 DBus::Glib::BusDispatcher dispatcher;
 DBus::default_dispatcher = &dispatcher;
 dispatcher.attach(NULL);
 DBus::Connection bus = DBus::Connection::SessionBus();
 Can can0(bus, "can0");
 can0.getConfiguration()->StartInterface(250000);
 BOOST_REQUIRE(can0.getConfiguration()->LinkState() == true);
 BroadcastManager bcm;
 bcm.open("can0");

 uint16_t id1;
 uint8_t data1[8];
 uint8_t dlc1;



 if (bcm.receive(&id1, data1, &dlc1)) {
 BOOST_TEST_MESSAGE("yeyyyyyy");

 }


 can0.getConfiguration()->StopInterface();
 Can can1(bus, "can1");
 can1.getConfiguration()->StopInterface();
 g_main_loop_run(loop);
 BOOST_TEST_MESSAGE("Test passed!");
 }*/
/*int main() {
 auto loop = g_main_loop_new(NULL, false);

 DBus::Glib::BusDispatcher dispatcher;
 DBus::default_dispatcher = &dispatcher;
 dispatcher.attach(NULL);
 DBus::Connection bus = DBus::Connection::SessionBus();
 Can can0(bus, "can0");
 can0.getConfiguration()->StartInterface(250000);

 BroadcastManager bcm;

 //bcm.open("can0");

 //uint16_t id1;
 //uint8_t data1[8];
 //uint8_t dlc1;

 SocketCAN can("can0");


 auto can_frame_received = [](uint32_t id, uint8_t data[8], uint8_t dlc) {

 std::cout << "START frame received." << std::endl;
 };
 // When a CAN frame has been received, can_frame_received() will be
 // called.
 if (!can.receive(can_frame_received)) {
 return 1;
 }
 g_main_loop_run(loop);
 return 0;

 }*/

