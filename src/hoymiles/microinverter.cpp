#include <iostream>
#include <memory>
#include <string>

#include "modbus.h"

#include "microinverter.h"
#include "port.h"
#include "sunspec.h"

Microinverter::Microinverter(std::shared_ptr<class modbus> modbus, int startAddress, long long serialNumber) : sunspec(40000, modbus) {
	this->modbus = modbus;
	this->startAddress = startAddress;
	this->serialNumber = serialNumber;

	this->age = 0;
}

// void Microinverter::updatePorts(std::vector<std::string> &parametersToGet, bool allParameters) {
// 	int registersToRead = (this->ports.size() * 40) / 2;
// 	uint16_t registersJoined[registersToRead];

// 	int registerCount = this->modbus.get()->modbus_read_holding_registers(this->startAddress, registersToRead, registersJoined);
// }

void Microinverter::updateParameters(std::vector<std::string> &parametersToGet, bool allParameters) {
	int portsRead = 0;
	while (portsRead < this->ports.size()) {
		int portsToRead = 0;
		while (portsToRead * 0x0019 < (128 - 0x0019) && (portsToRead + portsRead) < this->ports.size()) {
			portsToRead++;
		}

		int registersToRead = (portsToRead * 0x0019);
		uint16_t registers[registersToRead];

		int registerCount;
		registerCount = this->modbus.get()->modbus_read_holding_registers(this->startAddress + (portsRead * 0x0019), registersToRead, registers);

		if (registerCount != 0) {
			this->age++;
			return;
		}

		for (int i{0}; i < portsToRead; i++) {
			this->ports.at(i).setParametersFromMicroinverterArray(registers, i * 0x0019);
		}

		portsRead += portsToRead;
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
	while (portsIterator != this->ports.end()) {
		result += portsIterator->getParameterByName("todayProduction").first.get()->getValue().first.i;
		portsIterator++;
	}

	return result;
}

long long Microinverter::getTotalProduction() {
	long long result{0};

	std::vector<Port>::iterator portsIterator = this->ports.begin();
	while (portsIterator != this->ports.end()) {
		result += portsIterator->getParameterByName("totalProduction").first.get()->getValue().first.i;
		portsIterator++;
	}

	return result;
}