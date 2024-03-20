#include <iostream>
#include <memory>
#include <string>

#include "modbus.h"

#include "microinverter.h"
#include "port.h"

Microinverter::Microinverter(std::shared_ptr<class modbus> modbus, long long serialNumber) {
	this->modbus = modbus;
	this->serialNumber = serialNumber;
}

void Microinverter::updatePorts(std::vector<std::string> &parametersToGet, bool allParameters) {
	std::vector<Port>::iterator portsIterator = this->ports.begin();
	while (portsIterator != this->ports.end()) {
		portsIterator->updateParameters(parametersToGet, allParameters);
		portsIterator++;
	}
}

void Microinverter::printPorts(std::vector<std::string> &parametersToGet, bool allParameters, bool shortNames) {
	std::vector<Port>::iterator portsIterator = this->ports.begin();
	while (portsIterator != this->ports.end()) {
		portsIterator->printParameters(parametersToGet, allParameters, shortNames);
		std::cout << std::endl;
		portsIterator++;
	}
}

long long Microinverter::getTodayProduction() {
	long long result{0};

	std::vector<Port>::iterator portsIterator = this->ports.begin();
	while(portsIterator != this->ports.end()) {
		if(portsIterator->getParameterByName("todayProduction").first->age > 0) {
			portsIterator->getParameterByName("todayProduction").first->updateValue(this->modbus, portsIterator->portStartAddress);
		}
		result += portsIterator->getParameterByName("todayProduction").first->getValue().first.i;
		portsIterator++;
	}

	return result;	
}

long long Microinverter::getTotalProduction() {
	long long result{0};

	std::vector<Port>::iterator portsIterator = this->ports.begin();
	while(portsIterator != this->ports.end()) {
		if(portsIterator->getParameterByName("totalProduction").first->age > 0) {
			portsIterator->getParameterByName("totalProduction").first->updateValue(this->modbus, portsIterator->portStartAddress);
		}
		result += portsIterator->getParameterByName("totalProduction").first->getValue().first.i;
		portsIterator++;
		portsIterator++;
	}

	return result;
}