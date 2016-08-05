#ifndef FMS_LOAD_CONFIG_H
#define FMS_LOAD_CONFIG_H

#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <SimpleJSON/JSON.h>
#include <tinyXML/tinyxml.h>

#include "UtilsConfig.hpp"

class LoadConfig {

private:
	std::map<uint32_t, PGN> allPGNs;
private:
	std::map<int, std::string> allPorts;

public:
	LoadConfig() {
		this->getAllCANParam();
		this->getPortsConfigParam();
	}
	;

public:
	~LoadConfig() {
	}
	;

private:
	void getAllCANParam() {

		TiXmlDocument doc(CAN_CONFIG_FILE_LOC);

		if (doc.Error() == true) {

			std::cout << "could not load can_config.xml file" << std::endl;
			exit(-1);
		}

		doc.LoadFile();

		TiXmlHandle docHandle(&doc);
		TiXmlElement* child2 =
				docHandle.FirstChild("CAN").FirstChild("FMS").Child("PGN", 0).ToElement();

		while (child2) {

			PGN pgn;
			pgn.HANDLER_CLASS_NAME = child2->Attribute("handler");
			std::stringstream strValue;
			strValue << child2->Attribute("value");
			unsigned int intValue;
			strValue >> intValue;
			pgn.NUMBER = intValue;

			if (std::string(child2->Attribute("status"), 9) == "activated") {
				pgn.STATUS = PGNStatus::ACTIVATED;
			} else if (std::string(child2->Attribute("status"), 13)
					== "not_activated") {
				pgn.STATUS = PGNStatus::NOT_ACTIVATED;

			} else {
				pgn.STATUS = PGNStatus::ERROR;
			}

			uint32_t x = strtoul(std::string(child2->Attribute("id"), 10).c_str(), NULL, 16);
			pgn.CAN_ID = x;

			allPGNs[pgn.CAN_ID] = pgn;
			child2 = child2->NextSiblingElement("PGN");
		}
	};

private:
	void getPortsConfigParam() {
		TiXmlDocument doc(PORTS_CONFIG_FILE_LOC);
		if (doc.Error() == true) {
			std::cout << "could not load can_config.xml file" << std::endl;
			exit(-1);
		}
		doc.LoadFile();
		TiXmlHandle docHandle(&doc);
		TiXmlElement* child =
				docHandle.FirstChild("port-list").Child("port", 0).ToElement();
		int i = 0;
		while (child) {

			allPorts[i] = child->GetText();

			child = child->NextSiblingElement("port");
			i++;

		}

	};

public:
	std::map<int, std::string> getAllPorts() {
		return this->allPorts;
	};

public:
	std::map<uint32_t, PGN> getAllPGNs() {
		return this->allPGNs;

	};
};
#endif
