#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

#include "portParameters.h"

PortParameterMicroinverterSerialNumber::PortParameterMicroinverterSerialNumber() : PortParameterInt("microinverterSerialNumber", "mSN", 0x0001, 6) {}

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
	this->value.i = std::stoll(readValueString);
}

PortParameterPortNumber::PortParameterPortNumber() : PortParameterInt("portNumber", "pN", 0x0007, 1) {}

void PortParameterPortNumber::setValueFromRegisters(uint16_t *readArray, int registerCount) {
	if (registerCount > 0) {
		this->value.i = readArray[0];
		std::stringstream valueStringStream;
		valueStringStream << std::hex << this->value.i;
		this->value.i = valueStringStream.str().at(0) - '0';
	}
}

PortParameterPvVoltage::PortParameterPvVoltage() : PortParameterFloat("pvVoltage", "pvU", 1, 0x0008, 2) {}

PortParameterPvCurrentMi::PortParameterPvCurrentMi() : PortParameterFloat("pvCurrentMI", "pvIMI", 1, 0x000a, 2) {}

PortParameterPvCurrentHm::PortParameterPvCurrentHm() : PortParameterFloat("pvCurrentHM", "pvIHM", 2, 0x000a, 2) {}

PortParameterGridVoltage::PortParameterGridVoltage() : PortParameterFloat("gridVoltage", "gU", 1, 0x000c, 2) {}

PortParameterGridFrequency::PortParameterGridFrequency() : PortParameterFloat("gridFrequency", "gF", 2, 0x000e, 2) {}

PortParameterPvPower::PortParameterPvPower() : PortParameterFloat("pvPower", "pvP", 1, 0x0010, 2) {}

PortParameterTodayProduction::PortParameterTodayProduction() : PortParameterInt("todayProduction", "tdP", 0x0012, 2) {}

PortParameterTotalProduction::PortParameterTotalProduction() : PortParameterInt("totalProduction", "ttP", 0x0014, 4) {}

PortParameterTemperature::PortParameterTemperature() : PortParameterFloat("temperature", "t", 1, 0x0018, 2) {}

PortParameterOperatingStatus::PortParameterOperatingStatus() : PortParameterInt("operatingStatus", "oS", 0x001a, 2) {}

PortParameterAlarmCode::PortParameterAlarmCode() : PortParameterInt("alarmCode", "aC", 0x001c, 2) {}

PortParameterAlarmCount::PortParameterAlarmCount() : PortParameterInt("alarmCount", "aCnt", 0x001e, 2) {}

PortParameterLinkStatus::PortParameterLinkStatus() : PortParameterInt("linkStatus", "lS", 0x020, 2) {}