#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "modbus.h"

#include "port.h"
#include "portParameters.h"

Port::Port(std::shared_ptr<modbus_t *> modbus_context, std::mutex *modbus_context_mutex, uint16_t portStartAddress) {
	this->modbus_context = modbus_context;
	this->modbus_context_mutex = modbus_context_mutex;

	this->readArraySize = 34;
	this->readArray = new uint16_t[this->readArraySize];

	this->portStartAddress = portStartAddress;

	this->populateParameters();
}

void Port::populateParameters() {
	this->parameters.push_back(std::make_shared<PortParameterMicroinverterSerialNumber>());

	this->parameters.push_back(std::make_shared<PortParameterPortNumber>());

	this->parameters.push_back(std::make_shared<PortParameterPvVoltage>());

	this->parameters.push_back(std::make_shared<PortParameterPvCurrentMi>());

	this->parameters.push_back(std::make_shared<PortParameterPvCurrentHm>());

	this->parameters.push_back(std::make_shared<PortParameterGridVoltage>());

	this->parameters.push_back(std::make_shared<PortParameterGridFrequency>());

	this->parameters.push_back(std::make_shared<PortParameterPvPower>());

	this->parameters.push_back(std::make_shared<PortParameterTodayProduction>());

	this->parameters.push_back(std::make_shared<PortParameterTotalProduction>());

	this->parameters.push_back(std::make_shared<PortParameterTemperature>());

	this->parameters.push_back(std::make_shared<PortParameterOperatingStatus>());

	this->parameters.push_back(std::make_shared<PortParameterAlarmCode>());

	this->parameters.push_back(std::make_shared<PortParameterAlarmCount>());

	this->parameters.push_back(std::make_shared<PortParameterLinkStatus>());
}

void Port::updateParameters() {
	int registerCount;

	modbus_context_mutex->lock();
	registerCount = modbus_read_registers(*modbus_context.get(), portStartAddress - 1, this->readArraySize, this->readArray);
	modbus_context_mutex->unlock();

	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator{this->parameters.begin()};
	while (parametersIterator != this->parameters.end()) {
		parametersIterator->get()->updateValue(this->readArray, this->readArraySize, registerCount);
		parametersIterator++;
	}
}