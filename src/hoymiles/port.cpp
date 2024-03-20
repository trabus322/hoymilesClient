#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "modbus.h"

#include "port.h"
#include "portParameters.h"

Port::Port(std::shared_ptr<class modbus> modbus, uint16_t portStartAddress) {
	this->modbus = modbus;

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

std::pair<std::shared_ptr<PortParameter>, bool> Port::getParameterByName(std::string name) {
	std::pair<std::shared_ptr<PortParameter>, bool> result;
	result.second = false;

	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->parameters.begin();
	while (parametersIterator != this->parameters.end() && !result.second) {
		if (parametersIterator->get()->name == name) {
			result.first = *parametersIterator;
			result.second = true;
		}
		parametersIterator++;
	}

	return result;
}

void Port::fixCurrent() {
	if (this->currentFixed) {
		return;
	}

	if (!this->getParameterByName("pvCurrentMI").second != !this->getParameterByName("pvCurrentHM").second) {
		this->currentFixed = true;
		return;
	}

	if (this->getParameterByName("pvVoltage").second && this->getParameterByName("pvPower").second) {
		if (this->getParameterByName("pvCurrentMI").second && this->getParameterByName("pvCurrentHM").second) {
			if(this->getParameterByName("pvPower").first->getValue().first.f > this->getParameterByName("pvVoltage").first->getValue().first.f * this->getParameterByName("pvCurrentMI").first->getValue().first.f) {
				this->parameters.erase(std::find(this->parameters.begin(), this->parameters.end(), this->getParameterByName("pvCurrentHM").first));
			}
			else {
				this->parameters.erase(std::find(this->parameters.begin(), this->parameters.end(), this->getParameterByName("pvCurrentM").first));
			}
			this->currentFixed = true;
		}
	} 
}

void Port::updateParameters(std::vector<std::string> &parametersToGet, bool allParameters) {
	if (allParameters && parametersToGet.size() < this->parameters.size()) {
		std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->parameters.begin();
		while (parametersIterator != this->parameters.end()) {
			if (std::find(parametersToGet.begin(), parametersToGet.end(), parametersIterator->get()->name) == parametersToGet.end()) {
				parametersToGet.push_back(parametersIterator->get()->name);
			}
			parametersIterator++;
		}
	}

	std::vector<std::string>::iterator parametersToGetIterator = parametersToGet.begin();
	while (parametersToGetIterator != parametersToGet.end()) {
		std::pair<std::shared_ptr<PortParameter>, bool> parameterPair;

		parameterPair = this->getParameterByName(*parametersToGetIterator);
		if (parameterPair.second) {
			parameterPair.first->updateValue(this->modbus, this->portStartAddress);
		}

		this->fixCurrent();
		
		parametersToGetIterator++;
	}
}

void Port::printParameters(std::vector<std::string> &parametersToGet, bool allParameters) {
	if (allParameters && parametersToGet.size() < this->parameters.size()) {
		std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->parameters.begin();
		while (parametersIterator != this->parameters.end()) {
			if (std::find(parametersToGet.begin(), parametersToGet.end(), parametersIterator->get()->name) != parametersToGet.end()) {
				parametersToGet.push_back(parametersIterator->get()->name);
			}
			parametersIterator++;
		}
	}

	std::vector<std::string>::iterator parametersToGetIterator = parametersToGet.begin();
	if (parametersToGetIterator != parametersToGet.end()) {
		std::cout << "|";
	}

	while (parametersToGetIterator != parametersToGet.end()) {
		std::pair<std::shared_ptr<PortParameter>, bool> parameterPair;

		parameterPair = this->getParameterByName(*parametersToGetIterator);
		if (parameterPair.second) {
			std::cout << " " << parameterPair.first->name << ": " << parameterPair.first->getOutputValue() << " |";
		}
		parametersToGetIterator++;
	}
}