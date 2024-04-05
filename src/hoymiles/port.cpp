#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "modbus.h"

#include "port.h"
#include "portParameters.h"

Port::Port(int portStartAddress) {
	this->portStartAddress = portStartAddress;

	this->statusPortStartAddress = (((this->portStartAddress - 0x4000) / 0x0019) * 6) + 0xd006;

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


	this->statusParameters.push_back(std::make_shared<PortParameterOnOff>());

	this->statusParameters.push_back(std::make_shared<PortParameterLimitActivePower>());
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

std::pair<std::shared_ptr<PortParameter>, bool> Port::getStatusByName(std::string name) {
	std::pair<std::shared_ptr<PortParameter>, bool> result;
	result.second = false;

	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->statusParameters.begin();
	while (parametersIterator != this->statusParameters.end() && !result.second) {
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
			if (this->getParameterByName("pvPower").first->getValue().first.f > this->getParameterByName("pvVoltage").first->getValue().first.f * this->getParameterByName("pvCurrentMI").first->getValue().first.f) {
				this->parameters.erase(std::find(this->parameters.begin(), this->parameters.end(), this->getParameterByName("pvCurrentHM").first));
			} else {
				this->parameters.erase(std::find(this->parameters.begin(), this->parameters.end(), this->getParameterByName("pvCurrentMI").first));
			}
			this->currentFixed = true;
		}
	}
}

// void Port::increaseParametersAge() {
// 	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->parameters.begin();
// 	while(parametersIterator != this->parameters.end()) {
// 		parametersIterator->get()->age++;
// 		parametersIterator++;
// 	}
// }

// void Port::updateParameters(std::vector<std::string> &parametersToGet, bool allParameters) {
// 	this->increaseParametersAge();

// 	if (allParameters && parametersToGet.size() < this->parameters.size()) {
// 		std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->parameters.begin();
// 		while (parametersIterator != this->parameters.end()) {
// 			if (std::find(parametersToGet.begin(), parametersToGet.end(), parametersIterator->get()->name) == parametersToGet.end()) {
// 				parametersToGet.push_back(parametersIterator->get()->name);
// 			}
// 			parametersIterator++;
// 		}
// 	}

// 	std::vector<std::string>::iterator parametersToGetIterator = parametersToGet.begin();
// 	while (parametersToGetIterator != parametersToGet.end()) {
// 		std::pair<std::shared_ptr<PortParameter>, bool> parameterPair;

// 		parameterPair = this->getParameterByName(*parametersToGetIterator);
// 		if (parameterPair.second) {
// 			parameterPair.first->updateValue(this->modbus, this->portStartAddress);
// 		}

// 		this->fixCurrent();

// 		parametersToGetIterator++;
// 	}
// }

void Port::setParametersFromMicroinverterArray(uint16_t *registers, int addressOffset) {
	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->parameters.begin();
	while (parametersIterator != this->parameters.end()) {
		parametersIterator->get()->getValueFromRegisters(registers, addressOffset);
		parametersIterator++;
	}
	this->fixCurrent();
}

void Port::setStatusesFromMicroinverterArray(uint16_t *registers, int addressOffset) {
	std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->statusParameters.begin();
	while (parametersIterator != this->statusParameters.end()) {
		parametersIterator->get()->getValueFromRegisters(registers, addressOffset);
		parametersIterator++;
	}
}

void Port::printParameters(std::vector<std::string> &parametersToGet, bool allParameters, bool shortNames) {
	if (allParameters && parametersToGet.size() < this->parameters.size()) {
		std::vector<std::shared_ptr<PortParameter>>::iterator parametersIterator = this->parameters.begin();
		while (parametersIterator != this->parameters.end()) {
			std::string temp{parametersIterator->get()->name};
			if (std::find(parametersToGet.begin(), parametersToGet.end(), parametersIterator->get()->name) == parametersToGet.end()) {
				parametersToGet.push_back(parametersIterator->get()->name);
			}
			parametersIterator++;
		}
	}

	std::vector<std::string>::iterator parametersToGetIterator = parametersToGet.begin();
	if (parametersToGetIterator != parametersToGet.end()) {
		std::cout << "    |";
	}

	while (parametersToGetIterator != parametersToGet.end()) {
		if (*parametersToGetIterator == "microinverterSerialNumber") {
			parametersToGetIterator++;
			continue;
		}
		std::pair<std::shared_ptr<PortParameter>, bool> parameterPair;

		parameterPair = this->getParameterByName(*parametersToGetIterator);
		if (parameterPair.second) {
			std::cout << " ";
			if (shortNames) {
				std::cout << parameterPair.first->shortName;
			} else {
				std::cout << parameterPair.first->name;
			}
			std::cout << ": " << parameterPair.first->getOutputValue() << " |";
		}
		parametersToGetIterator++;
	}
}

void Port::turnOff(class modbus &modbus) { this->getStatusByName("onOff").first.get()->writeValue(0, modbus, this->statusPortStartAddress); }

bool Port::isOff(class modbus &modbus) {
	if(this->getStatusByName("onOff").first.get()->getValue().first.i == 1) {
		return true;
	}
	else {
		return false;
	}
}