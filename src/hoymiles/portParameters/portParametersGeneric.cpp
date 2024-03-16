#include <cmath>

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

void PortParameter::setValueFromRegisters(uint16_t *readArray, int registerCount) {}

std::pair<PortParameter::PortParameterValue, PortParameter::PortParameterValueType> PortParameter::getValue() {
    return std::pair<PortParameter::PortParameterValue, PortParameter::PortParameterValueType>(this->value, this->valueType);
}

std::string PortParameter::getOutputValue() {}

void PortParameter::updateValue(modbus_t *modbus_context, uint16_t portStartAddress) {
	uint16_t readArray[this->registerSize];
	int registerCount;
	registerCount = modbus_read_registers(modbus_context, portStartAddress + this->parameterAddressOffset, this->registerSize, readArray);

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

void PortParameterFloat::setValueFromRegisters(uint16_t *readArray, int registerCount) {
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

void PortParameterInt::setValueFromRegisters(uint16_t *readArray, int registerCount) {
	uint16_t readValue;
	for (int i{0}; i < registerCount; i++) {
		readValue = (readArray[i] & 0xFF00);
		this->value.i += readValue * std::pow(10, ((registerCount * 2) - ((registerCount - i) * 2)));

		readValue = readArray[i] & 0x00FF;
		this->value.i += readValue * std::pow(10, ((registerCount * 2) - ((registerCount - i) * 2)));
	}
}

std::string PortParameterInt::getOutputValue() {
	return std::to_string(this->value.i);
}