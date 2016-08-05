#ifndef SPN_TYPE_FINDER_H
#define SPN_TYPE_FINDER_H

#include <iostream>
#include <string>

#include "Config/UtilsConfig.hpp"
#include "FMS/FMSAcceleratorPedalPositionHandler.hpp"
#include "FMS/FMSActivateEngineDurationhandler.hpp"
#include "FMS/FMSAMBHandler.hpp"
#include "FMS/FMSCCVSHandler.hpp"
#include "FMS/FMSDashDisplayHandler.hpp"
#include "FMS/FMSDoorHandler.hpp"
#include "FMS/FMSFuelHandler.hpp"
#include "FMS/FMSGearHandler.hpp"
#include "FMS/FMSTimeDateHandler.hpp"
#include "FMS/FMSVehiculeDistanceHandler.hpp"
#include "FMS/FMSFuelConsumptionHandler.hpp"
#include "FMS/FMSEngineHandler.hpp"
#include "FMS/FMSEngineLiquideTemperatureHandler.hpp"


using namespace std;

class CANFrameHandler {
public:
	static void SPNHandler(PGN* pgn, FMSHandler* Handler) {

		if (pgn->STATUS == PGNStatus::ACTIVATED) {

			if (pgn->HANDLER_CLASS_NAME == "FMSTimeDateHandler") {
				FMSTimeDateHandler * fms =
						static_cast<FMSTimeDateHandler*>(Handler);
				TIME_VALUE_STRING = fms->getTimeFullFormatString();


				//for pedale and engine loading handling

			} else if (pgn->HANDLER_CLASS_NAME
					== "FMSAcceleratorPedalPositionHandler") {

				FMSAcceleratorPedalPositionHandler * fms =
						static_cast<FMSAcceleratorPedalPositionHandler*>(Handler);
				stringstream ss;
				ss << fms->AcceleratorPedalRatio();
				PEDAL_VALUE_STRING = ss.str();

				int a = fms->EngineLoadValue();

				stringstream ss1;
				ss1 << a;
				ENGINE_LOAD_VALUE_STRING = ss1.str();

			}
			//for gear loading handling

			else if (pgn->HANDLER_CLASS_NAME == "FMSGearHandler") {

				FMSGearhandler * fms = static_cast<FMSGearhandler*>(Handler);

				stringstream ss;
				ss << fms->calculateCurrentGear();
				stringstream ss1;
				ss1 << fms->calculateSelectedGear();
				CURRENT_GEAR_VALUE_STRING = ss.str();
				SELECTED_GEAR_VALUE_STRING = ss1.str();

			}
			//for door loading handling
			else if (pgn->HANDLER_CLASS_NAME == "FMSDoorHandler") {
				FMSDoorHandler * fms = static_cast<FMSDoorHandler*>(Handler);

				if (fms->checkErrorDoorsState() == false
						&& fms->checkNotAvailableDoorsState() == false) {
					if (fms->calculateAtLeastOneDoorOpenStatus() == false
							&& fms->calculateAllClosedDoorsStatus() == true) {
						DOOR_VALUE_STRING = DOOR_VALUE_CLOSED_STRING;
					}
					if (fms->calculateAtLeastOneDoorOpenStatus() == true) {

						DOOR_VALUE_STRING = DOOR_VALUE_OPEN_STRING;
					}
				} else {
					DOOR_VALUE_STRING = DOOR_VALUE_NOT_AVAILABLE_STRING;
				}
			} else if (pgn->HANDLER_CLASS_NAME== "FMSVehiculeDistanceHandler") {

				FMSVehiculeDistanceHandler * fms = static_cast<FMSVehiculeDistanceHandler*>(Handler);
				stringstream ss;
				ss << fms->calculateTotalDistance();
				TOTAL_VEHICULE_DISTANCE_VALUE_STRING = ss.str();
			}

			else if (pgn->HANDLER_CLASS_NAME
					== "FMSActivateEngineDurationhandler") {
				/*FMSActivateEngineDurationhandler * fms =
				 static_cast<FMSActivateEngineDurationhandler*>(Handler);
				 << fms->calculateTotalEngineWorkDuration() << std::endl;*/
			}

			else if (pgn->HANDLER_CLASS_NAME == "FMSCCVSHandler") {
				FMSCCVSHandler * fms = static_cast<FMSCCVSHandler*>(Handler);

				stringstream ss;
				ss << fms->calculateWheelSpeed();
				SPEED_VALUE_STRING = ss.str();
				if (fms->getBrakeSwitchState() == BrakeState::NOT_PRESSED) {
					BRAKE_STATE_VALUE_STRING = BRAKE_STATE_NOT_PRESSED_STRING;
				} else if (fms->getBrakeSwitchState() == BrakeState::PRESSED) {
					BRAKE_STATE_VALUE_STRING = BRAKE_STATE_PRESSED_STRING;
				} else {
					BRAKE_STATE_VALUE_STRING = BRAKE_STATE_UNKNOWN_STRING;
				}

				if (fms->getParkingBrakeSwitchState()
						== ParkingBrakeState::NOT_SET) {
					PARKING_BRAKE_STATE_VALUE_STRING =PARKING_BRAKE_STATE_NOT_SET_STRING;
				} else if (fms->getParkingBrakeSwitchState()== ParkingBrakeState::SET) {
					PARKING_BRAKE_STATE_VALUE_STRING =PARKING_BRAKE_STATE_SET_STRING;
				} else {
					PARKING_BRAKE_STATE_VALUE_STRING =
							PARKING_BRAKE_STATE_UNKNOWN_STRING;

				}

			} else if (pgn->HANDLER_CLASS_NAME == "FMSFuelHandler") {
				FMSFuelHandler * fms = static_cast<FMSFuelHandler*>(Handler);
				float tmp = fms->calculateFuelRate();
				stringstream ss;
				ss << tmp;
				FUEL_RATE_VALUE_STRING = ss.str();
				tmp = fms->calculateFuelEconomy();
				stringstream ss1;
				ss1 << tmp;
				INSTANTANEOUS_FUEL_ECONOMY_STRING_VALUE_STRING = ss1.str();

			} else if (pgn->HANDLER_CLASS_NAME == "FMSAMBHandler") {
				/*FMSAMBHandler * fms = static_cast<FMSAMBHandler*>(Handler);
				 cout << "outside Temperature:" << fms->calculateAMB()<< std::endl;*/
			} else if (pgn->HANDLER_CLASS_NAME == "FMSDashDisplayHandler") {
				FMSDashDisplayHandler * fms =
						static_cast<FMSDashDisplayHandler*>(Handler);
				stringstream ss;
				ss << fms->calculateFuelLevel();
				FUEL_VALUE_STRING = ss.str();
			} else if (pgn->HANDLER_CLASS_NAME == "FMSFuelConsumptionHandler") {
				FMSFuelConsumptionHandler * fms =
						static_cast<FMSFuelConsumptionHandler*>(Handler);
				float tmp = fms->calculateEngineTotalFuelUsed();
				stringstream ss;
				ss << tmp;
				FUEL_CONSUMPTION_VALUE_STRING = ss.str();
			}

			  else if (pgn->HANDLER_CLASS_NAME == "FMSEngineHandler") {
				FMSEngineHandler * fms = static_cast<FMSEngineHandler*>(Handler);
				uint8_t tmp = fms->calculateActualPercentTorque();
				stringstream ss;
				ss << unsigned(tmp);
				ACTUAL_ENGINE_TORQUE_VALUE_STRING = ss.str();
				float tmp1 = fms->calculateEngineSpeed();
				stringstream ss1;
				ss1 << tmp1;
				ACTUAL_ENGINE_SPEED_VALUE_STRING = ss1.str();
			}
			  else if (pgn->HANDLER_CLASS_NAME== "FMSEngineLiquideTemperatureHandler") {
				FMSEngineLiquideTemperatureHandler * fms =static_cast<FMSEngineLiquideTemperatureHandler*>(Handler);
				uint8_t tmp = fms->calculateEngineTemperature();

				stringstream ss;
				ss << unsigned (tmp);
				ENGINE_TEMPERATURE_VALUE_STRING=ss.str();
			}
		}
	}
};
#endif
