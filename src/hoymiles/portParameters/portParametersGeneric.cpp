#include <cmath>
#include <memory>
#include <mutex>
#include <sstream>
#include <iomanip>

#include "modbus.h"

#include "portParametersGeneric.h"

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

std::string PortParameter::getOutputValue() {
	return "yeet";
}

void PortParameter::updateValue(std::shared_ptr<class modbus> modbus, uint16_t portStartAddress) {
	uint16_t readArray[this->registerSize];
	int registerCount;
	
	// modbus_context_mutex->lock();
	registerCount = modbus.get()->modbus_read_holding_registers(portStartAddress + this->parameterAddressOffset, this->registerSize, readArray);
	// modbus_context_mutex->unlock();

	if(registerCount != 0){
		this->age++;
	}
	else{
		registerCount = this->registerSize;
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
	std::string separator{"_age"};
	std::stringstream valueStringStream;
	valueStringStream << std::fixed << std::setprecision(this->decimalPlaces) << this->value.f;
	return valueStringStream.str().append(separator.append(std::to_string(this->age)));
}

PortParameterInt::PortParameterInt(std::string name, uint16_t parameterAddressOffset, int registerSize) : PortParameter(name, parameterAddressOffset, registerSize) {
    this->valueType = Int;

    this->value.i = 0;
}

void PortParameterInt::setValueFromRegisters(uint16_t *readArray, int registerCount) {
	uint16_t readValue;
	std::string readValueString = "";
	registerCount = std::ceil(registerCount/2);
	for (int i{0}; i < registerCount; i++) {
		readValue = readArray[i];
		readValueString.append(std::to_string(readValue));
	}
	this->value.i = std::stoll(readValueString);
}

std::string PortParameterInt::getOutputValue() {
	std::string separator{"_age"};
	return std::to_string(this->value.i).append(separator.append(std::to_string(this->age)));
}