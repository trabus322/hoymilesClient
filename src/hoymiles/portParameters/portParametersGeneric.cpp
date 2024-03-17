#include <cmath>
#include <memory>
#include <mutex>

#include "modbus.h"

#include "portParametersGeneric.h"

struct _modbus;
typedef _modbus modbus_t;

PortParameter::PortParameter(std::string name, uint16_t parameterAddressOffset, int registerSize) {
	this->name = name;

	this->parameterAddressOffset = parameterAddressOffset;
	this->registerSize = registerSize;

    this->age = 0;
}

PortParameter::~PortParameter() {}

void PortParameter::setValueFromRegisters(uint16_t *readArray, int readArraySize) {}

std::pair<PortParameter::PortParameterValue, PortParameter::PortParameterValueType> PortParameter::getValue() {
    return std::pair<PortParameter::PortParameterValue, PortParameter::PortParameterValueType>(this->value, this->valueType);
}

std::string PortParameter::getOutputValue() {}

void PortParameter::updateValue(uint16_t *readArray, int readArraySize, int registerCount) {
	if(registerCount == -1){
		this->age++;
	}
	else{
		this->setValueFromRegisters(readArray, registerCount);
        this->age = 0;
	}
}

PortParameterFloat::PortParameterFloat(std::string name, int decimalPlaces, uint16_t parameterAddressOffset, int registerSize) : PortParameter(name, parameterAddressOffset, registerSize) {
	this->decimalPlaces = decimalPlaces;

	this->valueType = Float;

    this->value.f = 0;
}

void PortParameterFloat::setValueFromRegisters(uint16_t *readArray, int readArraySize) {
    float temp = readArray[0];
    temp = temp / std::pow(10, this->decimalPlaces);
	this->value.f = temp;
}

std::string PortParameterFloat::getOutputValue() {
	return std::to_string(this->value.f);
}

PortParameterInt::PortParameterInt(std::string name, uint16_t parameterAddressOffset, int registerSize) : PortParameter(name, parameterAddressOffset, registerSize) {
    this->valueType = Int;

    this->value.i = 0;
}

void PortParameterInt::setValueFromRegisters(uint16_t *readArray, int readArraySize) {
	uint16_t readValue;
	std::string readValueString = "";
	for (int i{0}; i < std::ceil(this->registerSize/2) && (this->parameterAddressOffset + i) < readArraySize; i++) {
		readValue = readArray[this->parameterAddressOffset + i];
		readValueString.append(std::to_string(readValue));
	}
	this->value.i = std::stol(readValueString);
}

std::string PortParameterInt::getOutputValue() {
	return std::to_string(this->value.i);
}