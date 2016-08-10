#ifndef MY_WEB_SOCKET_SERVER_H
#define MY_WEB_SOCKET_SERVER_H

#include <sstream>
#include <map>

#include "WebSocketServer.hpp"
#include "SystemFirewall.hpp"
#include <string>
#include "../Config/UtilsConfig.hpp"

enum START_JOURNEY_STATUS {
	OK = 0, NOK
};
enum RESQUEST_IDS {
	eECO_TRAINING_ACTION_START_TRAJECT = 0,
	eECO_TRAINING_ACTION_GEAR,
	eECO_TRAINING_ACTION_BREAK,
	eECO_TRAINING_ACTION_PEDAL,
	eECO_TRAINING_ENGINE_TEMPERATURE,
	eECO_TRAINING_ENGINE_RPM,
	eECO_TRAINING_ENGINE_SPEED,
	eECO_TRAINING_TIME,
	eECO_TRAINING_DISTANCE,
	eECO_TRAINING_FUEL_CONSUMPTION

};

class MyWebSocketServer: public WebSocketServer {

	int request;

public:
	MyWebSocketServer(int port) :
			WebSocketServer(port) {


	};
	~MyWebSocketServer() {

	};

	virtual void onConnect(int socketID) {
		std::cout << "server---->on connect" << std::endl;
	};

	virtual void onMessage(int socketID, const std::string& data) {
		wstring wideData;

		for (unsigned int i = 0; i < data.length(); ++i) {
			wideData += wchar_t(data[i]);
		}

		JSONValue *value = JSON::Parse(wideData.c_str());

		JSONObject root = value->AsObject();

		this->request = (int) root[L"request"]->AsNumber();

		std::string response = resquetHandler();
		this->send(socketID, response);

	}
	;

	virtual void onDisconnect(int socketID) {
		std::cout << "server---->on disconnect" << std::endl;
	};

	virtual void onError(int socketID, const std::string& message) {
		std::cout << "server---->error" << std::endl;
	};

public:
	void openFirewallPorts(DBus::Connection bus, uint16_t port) {

		// Create a proxy for the com.actia.platform.system.firewall interface
		SystemFirewall fw(bus);
		if (fw.State() == true) {
			fw.AppendRules( { { "wired", "tcp", port }, { "cellular", "tcp",
					port } });
		}
	};

public:
	void authorizeConfigPortFileConnection(DBus::Connection bus,
			std::map<int, std::string> allPorts) {

		// Create a proxy for the com.actia.platform.system.firewall interface
		SystemFirewall fw(bus);
		if (fw.State() == true) {

			map<int, std::string>::iterator it;

			for (it = allPorts.begin(); it != allPorts.end(); it++) {

				/*fw.AppendRules( { { "wired", "tcp", (int) it->second }, {
						"cellular", "tcp", (int) it->second } });*/

			}

		}
	};

private:
	std::string resquetHandler() {
		std::string responseWrapper = " ";
		switch (request) {

		case RESQUEST_IDS::eECO_TRAINING_ACTION_START_TRAJECT:
			responseWrapper = eco_training_start_traject_response();
			break;
		case RESQUEST_IDS::eECO_TRAINING_ACTION_GEAR:
			responseWrapper = eco_training_gear_response();
			break;
		case RESQUEST_IDS::eECO_TRAINING_ACTION_BREAK:
			responseWrapper = eco_training_brake_response();
			break;
		case RESQUEST_IDS::eECO_TRAINING_ACTION_PEDAL:
			responseWrapper = eco_training_pedal_pos_response();
			break;

		case RESQUEST_IDS::eECO_TRAINING_ENGINE_TEMPERATURE:
			responseWrapper = eco_training_engine_temperature_response();
			break;
		case RESQUEST_IDS::eECO_TRAINING_ENGINE_RPM:
			responseWrapper = eco_training_engine_rpm();
			break;

		case RESQUEST_IDS::eECO_TRAINING_ENGINE_SPEED:
			responseWrapper = eco_training_vehicule_speed();
			break;
		case RESQUEST_IDS::eECO_TRAINING_TIME:
			responseWrapper = eco_training_time();
			break;

		case RESQUEST_IDS::eECO_TRAINING_DISTANCE:
			responseWrapper = eco_training_distance();
			break;

		case RESQUEST_IDS::eECO_TRAINING_FUEL_CONSUMPTION:
			responseWrapper = eco_training_fuel_consumption();
			break;
		default:
			break;
		}
		return responseWrapper;
	};

private:
	std::string eco_training_start_traject_response() {

		std::string s =
				"{ \"request_id\": 0, \"eco_training_start_traject_status\": \"";

		if ((ACTUAL_ENGINE_SPEED_VALUE_STRING == "0")
				|| (ACTUAL_ENGINE_SPEED_VALUE_STRING == "")
				|| (TIME_VALUE_STRING == "")
				|| (FUEL_CONSUMPTION_VALUE_STRING == "")
				|| (TOTAL_VEHICULE_DISTANCE_VALUE_STRING == "")) {

			s = s + "1";
		}

		else {
			s = s + "0";
		}

		return (s + "\" } ");

	}
	;

private:
	std::string eco_training_gear_response() {

		std::string s ="{ \"request_id\": \"1\", \"eco_training_gear_status\": \""+ CURRENT_GEAR_VALUE_STRING + "\" } ";
		return s;
	}
	;

private:
	std::string eco_training_brake_response() {

		std::string s ="{ \"request_id\": \"2\", \"eco_training_break_status\": \""+ BRAKE_STATE_VALUE_STRING + "\" } ";
		return s;

	}
	;

private:
	std::string eco_training_pedal_pos_response() {

		std::string s ="{ \"request_id\": \"3\", \"eco_training_pedal_pos_status\": \""+ PEDAL_VALUE_STRING + "\" } ";

		return s;
	}
	;

private:
	std::string eco_training_engine_temperature_response() {

		std::string s =
				"{ \"request_id\": \"4\", \"eco_training_engine_temperature_status\": \""
						+ ENGINE_TEMPERATURE_VALUE_STRING + "\" } ";
		return s;

	}
	;

private:
	std::string eco_training_engine_rpm() {

		std::string s =
				"{ \"request_id\": \"5\", \"eco_training_engine_rpm_status\": \""
						+ ACTUAL_ENGINE_SPEED_VALUE_STRING + "\" } ";

		return s;

	}
	;
private:
	std::string eco_training_vehicule_speed() {

		std::string s =
				"{ \"request_id\": \"6\", \"eco_training_engine_vehicule_speed_status\": \""
						+ SPEED_VALUE_STRING + "\" } ";

		return s;

	}
	;
private:
	std::string eco_training_time() {

		std::string s =
				"{ \"request_id\": \"7\", \"eco_training_time_status\": \""
						+ TIME_VALUE_STRING + "\" } ";

		return s;
	};
private:
	std::string eco_training_distance() {

		std::string s =
				"{ \"request_id\": \"8\", \"eco_training_distance_status\": \""
						+ TOTAL_VEHICULE_DISTANCE_VALUE_STRING + "\" } ";
		return s;
	}
private:
	std::string eco_training_fuel_consumption() {

		std::string s =
				"{ \"request_id\": \"9\", \"eco_training_fuel_consumption_status\": \""
						+ FUEL_CONSUMPTION_VALUE_STRING + "\" } ";
		return s;
	}
};
#endif
