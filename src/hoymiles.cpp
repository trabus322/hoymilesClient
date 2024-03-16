#include "hoymiles.h"
#include "modbus.h"

#include <cmath>
#include <iostream>
#include <typeinfo>

Dtu::Dtu(const char *ip_address, int port) {
	this->modbus_context = modbus_new_tcp(ip_address, port);
	if (modbus_connect(this->modbus_context) == -1) {
		std::cerr << "conn_error";
		modbus_free(this->modbus_context);
		abort();
	}

	this->populateMicroinverters();
}

Dtu::~Dtu() {
	modbus_close(this->modbus_context);
	modbus_free(this->modbus_context);
}

void Dtu::populateMicroinverters() {
	uint16_t address{0x1000};
	Microinverter microinverter{this->modbus_context};
	this->microinverters.push_back(microinverter);
}

void Dtu::updateMicroinverters() {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while (microinvertersIterator != this->microinverters.end()) {
		microinvertersIterator->updatePorts();
		microinvertersIterator++;
	}
}



void Microinverter::populatePorts() {
	Port port{this->modbus_context, 0x1000};

	this->ports.push_back(port);
}

void Microinverter::updatePorts() {
	for(Port port : this->ports){
		port.updateParameters();
	}
}

Microinverter::Microinverter(modbus_t *modbus_context) {
	this->modbus_context = modbus_context;

	this->populatePorts();
}



Port::Port(modbus_t *modbus_context, uint16_t portStartAddress) {
	this->modbus_context = modbus_context;
	this->portStartAddress = portStartAddress;

	this->populateParameters();
}

void Port::populateParameters() {
	PortParameterFloat portParameterFloat{"gridVoltage", 0x0034, 2, 1};
	this->parameters.push_back(std::make_shared<PortParameterFloat>(portParameterFloat));

	PortParameterSerialNumber portParameterSerialNumber{};
	this->parameters.push_back(std::make_shared<PortParameterSerialNumber>(portParameterSerialNumber));
}

void Port::updateParameters() {
	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator{this->parameters.begin()};
	while(parametersIterator != this->parameters.end()){
		parametersIterator->get()->updateValue(this->modbus_context, this->portStartAddress);
		parametersIterator++;
	}
}

std::shared_ptr<PortParameter> Port::getParameterById(int i) {
	return this->parameters.at(i);
}



PortParameter::PortParameter(std::string name, uint16_t addressOffset, int registerSize) {
	this->name = name;

	this->addressOffset = addressOffset;
	this->registerSize = registerSize;
}

void PortParameter::updateValue(modbus_t *modbus_context, uint16_t microinverterAddress) {
	uint16_t readArray[this->registerSize];
	int registerCount;
	registerCount = modbus_read_registers(modbus_context, microinverterAddress + this->addressOffset, this->registerSize, readArray);
	registerCount = 2;
	readArray[0] = 2309;
	readArray[1] = 4354;
	if (registerCount == -1) {
		this->age++;
	} else {
		this->setValue(readArray, registerCount);
		this->age = 0;
	}
}

void PortParameter::setValue(uint16_t *readArray, int registerCount) {}



template<typename Type>
PortParameterTemplate<Type>::PortParameterTemplate(std::string name, uint16_t addressOffset, int registerSize) : PortParameter(name, addressOffset, registerSize) {}



PortParameterFloat::PortParameterFloat(std::string name, uint16_t addressOffset, int registerSize, int decimalPlaces) : PortParameterTemplate<float>(name, addressOffset, registerSize), PortParameter(name, addressOffset, registerSize) {
	this->decimalPlaces = decimalPlaces;
	this->value = 0;
}

void PortParameterFloat::setValue(uint16_t *readArray, int registerCount) {
	uint16_t readValue{readArray[0]};
	
	this->value = readValue / std::pow(10, this->decimalPlaces);
}

template<typename Type>
Type PortParameterTemplate<Type>::getValue() {
	return this->value;
}



PortParameterSerialNumber::PortParameterSerialNumber() : PortParameterTemplate<std::string>("serialNumber", 0x0001, 6), PortParameter("serialNumber", 0x0001, 6) {};

void PortParameterSerialNumber::setValue(uint16_t *readArray, int registerCount) {
	uint16_t readValue;
	int registerCountCorrected = std::min(3, registerCount);
	for(int i{0}; i<registerCountCorrected; i++){
		readValue = readArray[i] & 0xFF00;
		this->value+= readValue * std::pow(10, (registerCountCorrected-i)*2);

		readValue = readArray[i] & 0x00FF;
		this->value+= readValue * std::pow(10, ((registerCountCorrected-i)*2)-1);
	}
}