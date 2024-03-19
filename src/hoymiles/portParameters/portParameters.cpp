#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

#include "portParameters.h"

PortParameterMicroinverterSerialNumber::PortParameterMicroinverterSerialNumber() : PortParameterInt("microinverterSerialNumber", 0x0001, 6), PortParameter("microinverterSerialNumber", 0x0001, 6) {}

void PortParameterMicroinverterSerialNumber::setValueFromRegisters(uint16_t *readArray, int registerCount) {
	uint16_t readValue;
	std::string readValueString = "";
	registerCount = std::ceil(registerCount/2);
	for (int i{0}; i < registerCount; i++) {
		readValue = readArray[i];
		std::stringstream readValueStringStream;
		readValueStringStream << std::hex << readValue;
		readValueString.append(readValueStringStream.str());
	}
	this->value.i = std::stol(readValueString);
}

PortParameterPortNumber::PortParameterPortNumber() : PortParameterInt("portNumber", 0x0007, 1), PortParameter("portNumber", 0x0007, 1) {}

void PortParameterPortNumber::setValueFromRegisters(uint16_t *readArray, int registerCount) {
	if (registerCount > 0) {
		this->value.i = readArray[0];
		std::stringstream valueStringStream;
		valueStringStream << std::hex << this->value.i;
		this->value.i = valueStringStream.str().at(0) - '0';
	}
}

PortParameterPvVoltage::PortParameterPvVoltage() : PortParameterFloat("pvVoltage", 1, 0x0008, 2), PortParameter("pvVoltage", 0x0008, 2) {}

PortParameterPvCurrentMi::PortParameterPvCurrentMi() : PortParameterFloat("pvCurrentMI", 1, 0x000a, 2), PortParameter("pvCurrentMI", 0x000a, 2) {}

PortParameterPvCurrentHm::PortParameterPvCurrentHm() : PortParameterFloat("pvCurrentHM", 2, 0x000a, 2), PortParameter("pvCurrentHM", 0x000a, 2) {}

PortParameterGridVoltage::PortParameterGridVoltage() : PortParameterFloat("gridVoltage", 1, 0x000c, 2), PortParameter("gridVoltage", 0x000c, 2) {}

PortParameterGridFrequency::PortParameterGridFrequency() : PortParameterFloat("gridFrequency", 2, 0x000e, 2), PortParameter("gridFrequency", 0x000e, 2) {}

PortParameterPvPower::PortParameterPvPower() : PortParameterFloat("pvPower", 1, 0x0010, 2), PortParameter("pvPower", 0x0010, 2) {}

PortParameterTodayProduction::PortParameterTodayProduction() : PortParameterInt("todayProduction", 0x0012, 2), PortParameter("todayProduction", 0x0012, 2) {}

PortParameterTotalProduction::PortParameterTotalProduction() : PortParameterInt("totalProduction", 0x0014, 4), PortParameter("totalProduction", 0x0014, 4) {}

PortParameterTemperature::PortParameterTemperature() : PortParameterFloat("temperature", 1, 0x0018, 2), PortParameter("temperature", 0x0018, 2) {}

PortParameterOperatingStatus::PortParameterOperatingStatus() : PortParameterInt("operatingStatus", 0x001a, 2), PortParameter("operatingStatus", 0x001a, 2) {}

PortParameterAlarmCode::PortParameterAlarmCode() : PortParameterInt("alarmCode", 0x001c, 2), PortParameter("alarmCode", 0x001c, 2) {}

PortParameterAlarmCount::PortParameterAlarmCount() : PortParameterInt("alarmCount", 0x001e, 2), PortParameter("alarmCount", 0x001e, 2) {}

PortParameterLinkStatus::PortParameterLinkStatus() : PortParameterInt("linkStatus", 0x020, 2), PortParameter("linkStatus", 0x020, 2) {}