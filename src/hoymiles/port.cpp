#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <memory>

#include "modbus.h"

#include "port.h"
#include "portParameters.h"

Port::Port(modbus_t *modbus_context, uint16_t portStartAddress) {
	this->modbus_context = modbus_context;
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
	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator{this->parameters.begin()};
	while (parametersIterator != this->parameters.end()) {
		parametersIterator->get()->updateValue(this->modbus_context, this->portStartAddress);
		parametersIterator++;
	}
}