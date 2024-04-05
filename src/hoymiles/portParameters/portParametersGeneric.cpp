#include <cmath>
#include <memory>
#include <mutex>
#include <sstream>
#include <iomanip>

#include "modbus.h"

#include "portParametersGeneric.h"

PortParameter::PortParameter(std::string name, std::string shortName, std::string unit, bool r, bool w, uint16_t parameterAddressOffset, int registerSize) {
	this->name = name;
	this->shortName = shortName;
	this->unit = unit;

	this->r = r;
	this->w = w;

	this->parameterAddressOffset = parameterAddressOffset;
	this->registerSize = registerSize;
}

PortParameter::~PortParameter() {}

void PortParameter::getValueFromRegisters(uint16_t *readArray, int portOffset) {}

std::pair<PortParameter::PortParameterValue, PortParameter::PortParameterValueType> PortParameter::getValue() {
    return std::pair<PortParameter::PortParameterValue, PortParameter::PortParameterValueType>(this->value, this->valueType);
}

PortParameter& PortParameter::writeValue(uint16_t value, class modbus& modbus, int portStartAddress) {
	modbus.modbus_write_register(this->parameterAddressOffset + portStartAddress, value);
	return *this;
}

std::string PortParameter::getOutputValue() {
	return "yeet";
}

PortParameterFloat::PortParameterFloat(std::string name, std::string shortName, std::string unit, bool r, bool w, int decimalPlaces, uint16_t parameterAddressOffset, int registerSize) : PortParameter(name, shortName, unit, r, w, parameterAddressOffset, registerSize) {
	this->decimalPlaces = decimalPlaces;

	this->valueType = Float;

    this->value.f = 0;
}

void PortParameterFloat::getValueFromRegisters(uint16_t *registers, int addressOffset) {
	std::string readValueString{""};
	for(int i{0}; i<this->registerSize; i++) {
		std::stringstream readValueStringStream;
		readValueStringStream << (int) registers[addressOffset + this->parameterAddressOffset + i];
		readValueString.append(readValueStringStream.str().c_str());
	}
	this->value.f = std::stoll(readValueString) / std::pow(10, this->decimalPlaces);
}

std::string PortParameterFloat::getOutputValue() {
	std::stringstream valueStringStream;
	valueStringStream << std::fixed << std::setprecision(this->decimalPlaces) << this->value.f;
	return valueStringStream.str().append(this->unit.c_str());
}

PortParameterInt::PortParameterInt(std::string name, std::string shortName, std::string unit, bool r, bool w, uint16_t parameterAddressOffset, int registerSize) : PortParameter(name, shortName, unit, r, w, parameterAddressOffset, registerSize) {
    this->valueType = Int;

    this->value.i = 0;
}

void PortParameterInt::getValueFromRegisters(uint16_t *registers, int addressOffset) {
	std::string readValueString{""};
	for (int i{0}; i < this->registerSize; i++) {
		std::stringstream readValueStringStream;
		readValueStringStream << (int) registers[addressOffset + this->parameterAddressOffset + i];
		readValueString.append(readValueStringStream.str().c_str());
	}
	this->value.i = std::stoll(readValueString);
}

std::string PortParameterInt::getOutputValue() {
	return std::to_string(this->value.i).append(this->unit.c_str());
}