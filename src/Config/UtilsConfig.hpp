#ifndef UTILS_CONFIG_H
#define UTILS_CONFIG_H

#include <cstdint>
#include <iostream>
#include <string>

// can_config file description
 const char* CAN_CONFIG_FILE_LOC = "./can_config.xml";


// connection_config file description
 const char* PORTS_CONFIG_FILE_LOC="./connection_config.xml";
const std::string CONNECTION_CONFIG_FILE_PATH = "./";
const std::string CONNECTION_CONFIG_FILE_NAME = "connection_config.xml";



enum PGNStatus {

	ACTIVATED, NOT_ACTIVATED, ERROR
};

struct PGN {
	int NUMBER;
	uint32_t CAN_ID;
	std::string HANDLER_CLASS_NAME;
	enum PGNStatus STATUS;
};

std::string PRINT_STRING="\r";
std::string EMPTY_STRING="not defined";
std::string NOT_DEFINED_STRING="not defined";


std::string SEPARATOR_STRING="\n";


//doors string value for printing into console
std::string DOOR_STRING="doors: ";
std::string DOOR_VALUE_STRING=			    "";

std::string DOOR_VALUE_OPEN_STRING	=		"open  ";
std::string DOOR_VALUE_CLOSED_STRING=		"closed";
std::string DOOR_VALUE_NOT_AVAILABLE_STRING="NAV   ";


//time string value for printing into console
std::string TIME_STRING="time: ";
std::string TIME_VALUE_STRING=		"";

//current gear string value for printing into console
std::string CURRENT_GEAR_STRING="Current GEAR: ";
std::string CURRENT_GEAR_VALUE_STRING=	"";

//current gear string value for printing into console
std::string SELECTED_GEAR_STRING="Selected GEAR: ";
std::string SELECTED_GEAR_VALUE_STRING=	"";

//speed string value for printing into console
std::string SPEED_STRING="Speed: ";
std::string SPEED_UNIT_STRING="km/h";
std::string SPEED_VALUE_STRING=	"";

//Brake string value for printing into console
std::string BRAKE_STATE_STRING="Brake state: ";
std::string BRAKE_STATE_PRESSED_STRING=    "pressed";
std::string BRAKE_STATE_NOT_PRESSED_STRING="Not pressed";
std::string BRAKE_STATE_UNKNOWN_STRING="unknown    ";
std::string BRAKE_STATE_VALUE_STRING;

//Parking Brake string value for printing into console
std::string PARKING_BRAKE_STATE_STRING="Parking brake state: ";
std::string PARKING_BRAKE_STATE_SET_STRING=    "set    ";
std::string PARKING_BRAKE_STATE_NOT_SET_STRING="not set";
std::string PARKING_BRAKE_STATE_UNKNOWN_STRING=    "unknown";
std::string PARKING_BRAKE_STATE_VALUE_STRING;


//fuel string value for printing into console
std::string FUEL_STRING="Fuel level: ";
std::string FUEL_UNIT_STRING="%";
std::string FUEL_VALUE_STRING=	"";

//pedal position string value for printing into console
std::string PEDAL_STRING="Pedal pos: ";
std::string PEDAL_UNIT_STRING="%";
std::string PEDAL_VALUE_STRING=	"";

//Engine load string value for printing into console
std::string ENGINE_LOAD_STRING="Engine Load: ";
std::string ENGINE_LOAD_UNIT_STRING="%";
std::string ENGINE_LOAD_VALUE_STRING;

//Fuel consumption string value for printing into console
std::string FUEL_CONSUMPTION_STRING="Total fuel consumption: ";
std::string FUEL_CONSUMPTION_UNIT_STRING="L";
std::string FUEL_CONSUMPTION_VALUE_STRING;

//Fuel rate string value for printing into console
std::string FUEL_RATE_STRING="Fuel rate: ";
std::string FUEL_RATE_UNIT_STRING="L/h";
std::string FUEL_RATE_VALUE_STRING;

//Instantaneous Fuel Economy  string value for printing into console
std::string INSTANTANEOUS_FUEL_ECONOMY_STRING="Instantaneous Fuel Economy: ";
std::string INSTANTANEOUS_FUEL_ECONOMY_STRING_UNIT_STRING="km/L";
std::string INSTANTANEOUS_FUEL_ECONOMY_STRING_VALUE_STRING;

//Actual engine torque string value for printing into console
std::string ACTUAL_ENGINE_TORQUE_STRING="Actual engine torque: ";
std::string ACTUAL_ENGINE_TORQUE_UNIT_STRING="%";
std::string ACTUAL_ENGINE_TORQUE_VALUE_STRING;

//Actual engine speed string value for printing into console
std::string ACTUAL_ENGINE_SPEED_STRING="Actual engine speed: ";
std::string ACTUAL_ENGINE_SPEED_UNIT_STRING="rpm";
std::string ACTUAL_ENGINE_SPEED_VALUE_STRING="0";

//Total vehicule distance value for printing into console
std::string TOTAL_VEHICULE_DISTANCE_STRING="Total vehicule distance: ";
std::string TOTAL_VEHICULE_DISTANCE_UNIT_STRING="m";
std::string TOTAL_VEHICULE_DISTANCE_VALUE_STRING="";

//Engine temperature
std::string ENGINE_TEMPERATURE_STRING="Engine Temperature: ";
std::string ENGINE_TEMPERATURE_UNIT_STRING="°C";
std::string ENGINE_TEMPERATURE_VALUE_STRING;

#endif
