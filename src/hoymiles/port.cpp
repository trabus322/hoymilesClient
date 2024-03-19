#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <memory>

#include "modbus.h"

#include "port.h"
#include "portParameters.h"

Port::Port(std::shared_ptr<modbus_t*> modbus_context, uint16_t portStartAddress) {
	this->modbus_context = modbus_context;
	// this->modbus_context_mutex = modbus_context_mutex;

	this->portStartAddress = portStartAddress;

	this->currentFixed = false;

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

void Port::fixCurrent() {
	if(this->currentFixed) {
		return;
	}
	if(this->parameters.at(7).get()->getValue().first.f == 0) {
		return;
	}
	if(this->parameters.at(2).get()->getValue().first.f * this->parameters.at(4).get()->getValue().first.f < this->parameters.at(7).get()->getValue().first.f) {
		this->parameters.erase(this->parameters.begin() + 4);
	}
	else {
		this->parameters.erase(this->parameters.begin() + 3);
	}
	this->currentFixed = true;
}

void Port::updateParameters() {
	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator{this->parameters.begin()};
	while (parametersIterator != this->parameters.end()) {
		parametersIterator->get()->updateValue(this->modbus_context, this->portStartAddress);
		parametersIterator++;
	}
	this->fixCurrent();
}

void Port::printParameters() {
	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->parameters.begin();

	if(parametersIterator != this->parameters.end()) {
		std::cout << "|";
	}

	while(parametersIterator != this->parameters.end()) {
		std::cout << " " << parametersIterator->get()->name << ": " << parametersIterator->get()->getOutputValue() << " |";
		parametersIterator++;
	}
}