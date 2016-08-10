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

#include <actia/platform/can_proxy.h>
#include <Can/Can.hpp>
#include <Can/CanConfig.hpp>
#include <Can/SocketCan.h>
#include <dbus-c++/connection.h>
#include <dbus-c++/dispatcher.h>
#include <dbus-c++/glib-integration.h>
#include <glib/gmain.h>
#include <stdlib.h>
#include <cstdint>
#include <map>
#include <cstdlib>
#include <iostream>
#include "Config/LoadConfig.hpp"
#include "Config/UtilsConfig.hpp"
#include "FMS/FMSHandler.hpp"

#include "CANFrameHander.hpp"
#include "WebSocketServer/MyWebSocketServer.hpp"

#include <thread>

using namespace std;


void WebSocketServerWorker(DBus::Connection bus) {
	int port = 9001;
	MyWebSocketServer es = MyWebSocketServer(port);
	es.openFirewallPorts(bus, port);
	es.run();
};

void printConsoleWorker() {
	while (true) {
		PRINT_STRING = "\r";
		PRINT_STRING += TIME_STRING + TIME_VALUE_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += DOOR_STRING + DOOR_VALUE_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += CURRENT_GEAR_STRING + CURRENT_GEAR_VALUE_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += SELECTED_GEAR_STRING + SELECTED_GEAR_VALUE_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += SPEED_STRING + SPEED_VALUE_STRING + SPEED_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += BRAKE_STATE_STRING + BRAKE_STATE_VALUE_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += PARKING_BRAKE_STATE_STRING+ PARKING_BRAKE_STATE_VALUE_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += FUEL_STRING + FUEL_VALUE_STRING + FUEL_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += PEDAL_STRING + PEDAL_VALUE_STRING + PEDAL_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += ENGINE_LOAD_STRING + ENGINE_LOAD_VALUE_STRING + ENGINE_LOAD_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += FUEL_CONSUMPTION_STRING + FUEL_CONSUMPTION_VALUE_STRING + FUEL_CONSUMPTION_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += FUEL_RATE_STRING + FUEL_RATE_VALUE_STRING
				+ FUEL_RATE_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += INSTANTANEOUS_FUEL_ECONOMY_STRING
				+ INSTANTANEOUS_FUEL_ECONOMY_STRING_VALUE_STRING
				+ INSTANTANEOUS_FUEL_ECONOMY_STRING_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += ACTUAL_ENGINE_TORQUE_STRING
				+ ACTUAL_ENGINE_TORQUE_VALUE_STRING
				+ ACTUAL_ENGINE_TORQUE_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += ACTUAL_ENGINE_SPEED_STRING
				+ ACTUAL_ENGINE_SPEED_VALUE_STRING
				+ ACTUAL_ENGINE_SPEED_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += TOTAL_VEHICULE_DISTANCE_STRING
				+ TOTAL_VEHICULE_DISTANCE_VALUE_STRING
				+ TOTAL_VEHICULE_DISTANCE_UNIT_STRING;
		PRINT_STRING += SEPARATOR_STRING;
		PRINT_STRING += ENGINE_TEMPERATURE_STRING
				+ ENGINE_TEMPERATURE_VALUE_STRING
				+ ENGINE_TEMPERATURE_UNIT_STRING;
		std::cout << PRINT_STRING << std::flush;
		usleep(70000);
		std::system("clear");
	}
}

int main() {
	std::map<uint32_t, PGN> allPGNs;
	std::map<int, std::string> allPorts;
	LoadConfig *configuration = new LoadConfig();

	allPGNs = configuration->getAllPGNs();
	allPorts = configuration->getAllPorts();
	delete configuration;


	auto loop = g_main_loop_new(NULL, false);
	DBus::Glib::BusDispatcher dispatcher;
	DBus::default_dispatcher = &dispatcher;
	dispatcher.attach(NULL);
	DBus::Connection bus = DBus::Connection::SessionBus();
	Can can0(bus, "can0");
	Can can1(bus, "can1");

	can1.getConfiguration()->StopInterface();
	can0.getConfiguration()->StopInterface();
	usleep(700000);
	can0.getConfiguration()->StartInterface(250000);


	/*if (can0.getConfiguration()->LinkState() == false) {
		exit(-1);
	}*/

	for (std::map<uint32_t, PGN>::iterator it=allPGNs.begin(); it!=allPGNs.end(); ++it)
	{
		uint32_t masked_id1=it->first&0x00FFFF00;
	    std::cout <<std::hex <<masked_id1 << std::endl;
	}
	    FMSHandler * GenericFms = new FMSHandler();

	auto can_frame_received =
			[&allPGNs,&GenericFms](uint32_t id, uint8_t data[8], uint8_t dlc)
			{
		uint32_t masked_id=id&0x00FFFF00;
		//std::cout<<std::hex<<masked_id<<std::endl;


				if (allPGNs.count(masked_id))
				{

					GenericFms->setDATA(data);
					GenericFms->setDLC(dlc);
					GenericFms->setID(masked_id);
					PGN * detectedPGN = &allPGNs[masked_id];
					CANFrameHandler::SPNHandler(detectedPGN,GenericFms);
				}
			};
	SocketCAN socket("can0");
	if (!socket.receive(can_frame_received)) {
		return 1;
	}
	thread ConsoleUIThread(printConsoleWorker);
	std::thread WebSocketServerThread(WebSocketServerWorker, bus);

	g_main_loop_run(loop);
	return 0;
}

