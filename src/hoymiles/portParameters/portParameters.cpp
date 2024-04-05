#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

#include "portParameters.h"

PortParameterMicroinverterSerialNumber::PortParameterMicroinverterSerialNumber() : PortParameterInt("microinverterSerialNumber", "mSN", "", true, false, 0x0001, 3) {}

void PortParameterMicroinverterSerialNumber::getValueFromRegisters(uint16_t *registers, int addressOffset) {
	std::string readValueString = "";
	for (int i{0}; i < this->registerSize; i++) {
		std::stringstream readValueStringStream;
		readValueStringStream << std::hex << (int) ((registers[addressOffset + this->parameterAddressOffset + i] & 0xff00) >> 8);
		readValueStringStream << std::hex << (int) (registers[addressOffset + this->parameterAddressOffset + i] & 0x00ff);
		readValueString.append(readValueStringStream.str());
	}
	this->value.i = std::stoll(readValueString);
}

PortParameterPortNumber::PortParameterPortNumber() : PortParameterInt("portNumber", "pN", "", true, false, 0x0004, 1) {}

PortParameterPvVoltage::PortParameterPvVoltage() : PortParameterFloat("pvVoltage", "pvU", "V", true, false, 1, 0x0005, 1) {}

PortParameterPvCurrentMi::PortParameterPvCurrentMi() : PortParameterFloat("pvCurrentMI", "pvIMI", "A", true, false, 1, 0x0006, 1) {}

PortParameterPvCurrentHm::PortParameterPvCurrentHm() : PortParameterFloat("pvCurrentHM", "pvIHM", "A", true, false, 2, 0x0006, 1) {}

PortParameterGridVoltage::PortParameterGridVoltage() : PortParameterFloat("gridVoltage", "gU", "V", true, false, 1, 0x0007, 1) {}

PortParameterGridFrequency::PortParameterGridFrequency() : PortParameterFloat("gridFrequency", "gF", "Hz", true, false, 2, 0x0008, 1) {}

PortParameterPvPower::PortParameterPvPower() : PortParameterFloat("pvPower", "pvP", "W", true, false, 1, 0x0009, 1) {}

PortParameterTodayProduction::PortParameterTodayProduction() : PortParameterInt("todayProduction", "tdP", "Wh", true, false, 0x000A, 1) {}

PortParameterTotalProduction::PortParameterTotalProduction() : PortParameterInt("totalProduction", "ttP", "Wh", true, false, 0x000B, 2) {}

PortParameterTemperature::PortParameterTemperature() : PortParameterFloat("temperature", "t", "C", true, false, 1, 0x000D, 1) {}

PortParameterOperatingStatus::PortParameterOperatingStatus() : PortParameterInt("operatingStatus", "oS", "", true, false, 0x000e, 1) {}

PortParameterAlarmCode::PortParameterAlarmCode() : PortParameterInt("alarmCode", "aC", "", true, false, 0x000f, 1) {}

PortParameterAlarmCount::PortParameterAlarmCount() : PortParameterInt("alarmCount", "aCnt", "", true, false, 0x0010, 1) {}

PortParameterLinkStatus::PortParameterLinkStatus() : PortParameterInt("linkStatus", "lS", "", true, false, 0x011, 1) {}

PortParameterOnOff::PortParameterOnOff() : PortParameterInt("onOff", "of", "", true, true, 0x0000, 1) {}

PortParameterLimitActivePower::PortParameterLimitActivePower() : PortParameterInt("limitActivePower", "lAP", "", true, true, 0x0001, 1) {}