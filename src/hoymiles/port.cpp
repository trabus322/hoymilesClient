#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <memory>

#include "modbus.h"

#include "port.h"

Port::Port(modbus_t *modbus_context, uint16_t portStartAddress) {
	this->modbus_context = modbus_context;
	this->portStartAddress = portStartAddress;

	this->populateParameters();
}

void Port::populateParameters() {
	PortParameterFloat portParameterFloat{"gridVoltage", 1, 0x001a, 2};
	this->parameters.push_back(std::make_shared<PortParameterFloat>(portParameterFloat));
}

void Port::updateParameters() {
	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator{this->parameters.begin()};
	while (parametersIterator != this->parameters.end()) {
		parametersIterator->get()->updateValue(this->modbus_context, this->portStartAddress);
		parametersIterator++;
	}
}

PortParameter::PortParameter(std::string name, uint16_t parameterAddressOffset, int registerSize) {
	this->name = name;

	this->parameterAddressOffset = parameterAddressOffset;
	this->registerSize = registerSize;
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

	registerCount = 2;
	readArray[0] = 2312;
	readArray[1] = 5432;

	if(registerCount == -1){
		std::cerr << "read_error";
		return;
	}
	else{
		this->setValueFromRegisters(readArray, registerCount);
	}
}

PortParameterFloat::PortParameterFloat(std::string name, int decimalPlaces, uint16_t parameterAddressOffset, int registerSize) : PortParameter(name, parameterAddressOffset, registerSize) {
	this->decimalPlaces = decimalPlaces;

	this->valueType = Float;
}

void PortParameterFloat::setValueFromRegisters(uint16_t *readArray, int registerCount) {
	this->value.f = readArray[0] / std::pow(10, this->decimalPlaces);
}

std::string PortParameterFloat::getOutputValue() {
	return std::to_string(this->value.f);
}