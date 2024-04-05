#include <cmath>
#include <memory>
#include <mutex>
#include <sstream>
#include <iomanip>

#include "modbus.h"

#include "portParametersGeneric.h"

PortParameter::PortParameter(std::string name, std::string shortName, std::string unit, uint16_t parameterAddressOffset, int registerSize) {
	this->name = name;
	this->shortName = shortName;
	this->unit = unit;

	this->parameterAddressOffset = parameterAddressOffset;
	this->registerSize = registerSize;

    // this->age = 0;
}

PortParameter::~PortParameter() {}

void PortParameter::setValueFromRegisters(uint16_t *readArray, int portOffset) {}

std::pair<PortParameter::PortParameterValue, PortParameter::PortParameterValueType> PortParameter::getValue() {
    return std::pair<PortParameter::PortParameterValue, PortParameter::PortParameterValueType>(this->value, this->valueType);
}

std::string PortParameter::getOutputValue() {
	return "yeet";
}

// void PortParameter::updateValue(std::shared_ptr<class modbus> modbus, uint16_t portStartAddress) {
// 	uint16_t readArray[this->registerSize];
// 	int registerCount;
	
// 	registerCount = modbus.get()->modbus_read_holding_registers(portStartAddress + this->parameterAddressOffset, this->registerSize, readArray);

// 	if(registerCount != 0){
// 		this->age++;
// 	}
// 	else{
// 		registerCount = this->registerSize;
// 		this->setValueFromRegisters(readArray, registerCount);
//         this->age = 0;
// 	}
// }

PortParameterFloat::PortParameterFloat(std::string name, std::string shortName, std::string unit, int decimalPlaces, uint16_t parameterAddressOffset, int registerSize) : PortParameter(name, shortName, unit, parameterAddressOffset, registerSize) {
	this->decimalPlaces = decimalPlaces;

	this->valueType = Float;

    this->value.f = 0;
}

void PortParameterFloat::setValueFromRegisters(uint16_t *registers, int addressOffset) {
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

PortParameterInt::PortParameterInt(std::string name, std::string shortName, std::string unit, uint16_t parameterAddressOffset, int registerSize) : PortParameter(name, shortName, unit, parameterAddressOffset, registerSize) {
    this->valueType = Int;

    this->value.i = 0;
}

void PortParameterInt::setValueFromRegisters(uint16_t *registers, int addressOffset) {
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