#include <iostream>
#include <memory>
#include <string>

#include "modbus.h"

#include "microinverter.h"
#include "port.h"
#include "sunspec.h"

Microinverter::Microinverter(modbus_t *modbus, int startAddress, long long serialNumber) : sunspec(40000, modbus) {
	this->modbus = modbus;
	this->startAddress = startAddress;
	this->serialNumber = serialNumber;

	this->statusStartAddress = (((this->startAddress - 0x4000) / 0x0019) * 6) + 0xd006;

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
		registerCount = modbus_read_registers(this->modbus, this->startAddress + (portsRead * 0x0019), registersToRead, registers);

		if (registerCount == -1) {
			this->age++;
			return;
		}

		for (int i{0}; i < portsToRead; i++) {
			this->ports.at(i + portsRead).setParametersFromMicroinverterArray(registers, i * 0x0019);
		}

		portsRead += portsToRead;
	}
}

void Microinverter::updateStatusParameters() {
	int portsRead = 0;
	while (portsRead < this->ports.size()) {
		int portsToRead = 0;
		while (portsToRead * 6 < (128 - 6) && (portsToRead + portsRead) < this->ports.size()) {
			portsToRead++;
		}

		int registersToRead = (portsToRead * 6);
		uint16_t registers[registersToRead];

		int registerCount;
		registerCount = modbus_read_registers(this->modbus, this->statusStartAddress + (portsRead * 6), registersToRead, registers);

		if (registerCount == -1) {
			this->age++;
			return;
		}
		else {
			this->age = 0;
		}

		for (int i{0}; i < portsToRead; i++) {
			this->ports.at(i + portsRead).setStatusesFromMicroinverterArray(registers, i * 6);
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

void Microinverter::setStatus(std::vector<std::pair<int, uint16_t>> portsToSet, std::string statusName) {
	std::vector<std::pair<int, uint16_t>>::iterator portsToSetIterator = portsToSet.begin();
	while(portsToSetIterator != portsToSet.end()) {
		try {
			if(this->ports.at(portsToSetIterator->first).getStatusByName(statusName).second) {
				this->ports.at(portsToSetIterator->first).getStatusByName(statusName).first->writeValue(portsToSetIterator->second, this->modbus, this->statusStartAddress);
			}
		}
		catch(const std::out_of_range& outOfRange) {
			std::cerr << outOfRange.what() << std::endl;
		}
		portsToSetIterator++;
	}
}

void Microinverter::setStatusWholeMicroinverter(uint16_t value, std::string statusName) {
	if(this->ports.begin() != this->ports.end()) {
		if(this->ports.begin()->getStatusByName(statusName).second) {
			this->ports.begin()->getStatusByName(statusName).first.get()->writeValue(value, this->modbus, this->ports.begin()->statusPortStartAddress);
		}
	}
}