// #include <thread>
#include <iostream>
#include <memory>
#include <string>

#include "modbus.h"

#include "microinverter.h"
#include "port.h"

Microinverter::Microinverter(std::shared_ptr<class modbus> modbus, long serialNumber) {
	this->modbus = modbus;
	// this->modbus_context_mutex = modbus_context_mutex;

	this->serialNumber = serialNumber;
}

void Microinverter::updatePorts(std::vector<std::string> &parametersToGet, bool allParameters) {
	std::vector<Port>::iterator portsIterator = this->ports.begin();
	while(portsIterator != this->ports.end()) {
		portsIterator->updateParameters(parametersToGet, allParameters);
		portsIterator++;
	}
}

void Microinverter::printPorts(std::vector<std::string> &parametersToGet, bool allParameters) {
	std::cout << "Microinverter: " << this->serialNumber << std::endl;

	std::vector<Port>::iterator portsIterator = this->ports.begin();
	while(portsIterator != this->ports.end()) {
		portsIterator->printParameters(parametersToGet, allParameters);
		std::cout << std::endl;
		portsIterator++;
	}
}