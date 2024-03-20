#include <vector>
#include <iostream>
#include <string>

#include "modbus.h"

#include "dtu.h"
#include "microinverter.h"

#include "portParameters.h"


Dtu::Dtu(const char *ip_address, int port) {
	class modbus modbus{ip_address, (uint16_t) port};
	this->modbus = std::make_shared<class modbus>(modbus);

	if (!this->modbus.get()->modbus_connect()) {
		std::cerr << "conn_error" << std::endl;
		this->connected = false;
	}
	else {
		this->connected = true;
	}

	if(this->connected) {
		this->populateMicroinverters();
	}
}

bool Dtu::isConnected() {
	return this->connected;
}

Dtu::~Dtu() {
	this->modbus.get()->modbus_close();
}

void Dtu::populateMicroinverters() {
	uint16_t portStartAddress = 0x1000;
	uint16_t readArray[1];

	int registerCount;
	registerCount = this->modbus.get()->modbus_read_holding_registers(portStartAddress + 0x0021, 1, readArray);
	while(registerCount != -1 && readArray[0] == 0x700) {
		Port port{ this->modbus, portStartAddress };

		PortParameterMicroinverterSerialNumber portParameterMicroinverterSerialNumber{};
		portParameterMicroinverterSerialNumber.updateValue(this->modbus, portStartAddress);
		long serialNumber = portParameterMicroinverterSerialNumber.getValue().first.i;

		std::pair<bool, Microinverter*> getMicroinverterBySerialNumber = this->getMicroinverterBySerialNumber(serialNumber);
		if(getMicroinverterBySerialNumber.first) {
			getMicroinverterBySerialNumber.second->ports.push_back(port);
		}
		else {
			Microinverter microinverter{ this->modbus, serialNumber };
			this->microinverters.push_back(microinverter);
			this->microinverters.back().ports.push_back(port);
		}

		portStartAddress += 0x0028;

		registerCount = this->modbus.get()->modbus_read_holding_registers(portStartAddress + 0x0021, 1, readArray);
	}
}

std::pair<bool, Microinverter*> Dtu::getMicroinverterBySerialNumber(long serialNumber) {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while(microinvertersIterator != this->microinverters.end()) {
		if(microinvertersIterator->serialNumber == serialNumber) {
			return std::pair<bool, Microinverter*>(true, &*microinvertersIterator);
		}
		else{
			microinvertersIterator++;
		}
	}
	return std::pair<bool, Microinverter*>(false, &*microinvertersIterator);
}

void Dtu::updateMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long> &microinvertersToGet) {
	if(microinvertersToGet.empty()) {
		std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
		while(microinvertersIterator != this->microinverters.end()) {
			microinvertersToGet.push_back(microinvertersIterator->serialNumber);
			microinvertersIterator++;
		}
	}

	std::vector<long>::iterator microinvertersToGetIterator = microinvertersToGet.begin();
	while(microinvertersToGetIterator != microinvertersToGet.end()) {
		std::pair<bool, Microinverter*> microinverterPair = this->getMicroinverterBySerialNumber(*microinvertersToGetIterator);
		if(microinverterPair.first) {
			microinverterPair.second->updatePorts(parametersToGet, allParameters);
		}
		microinvertersToGetIterator++;
	}
}

void Dtu::printMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long> &microinvertersToGet) {
	if(microinvertersToGet.empty()) {
		std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
		while(microinvertersIterator != this->microinverters.end()) {
			microinvertersToGet.push_back(microinvertersIterator->serialNumber);
			microinvertersIterator++;
		}
	}

	std::vector<long>::iterator microinvertersToGetIterator = microinvertersToGet.begin();
	while(microinvertersToGetIterator != microinvertersToGet.end()) {
		std::pair<bool, Microinverter*> microinverterPair = this->getMicroinverterBySerialNumber(*microinvertersToGetIterator);
		if(microinverterPair.first) {
			microinverterPair.second->printPorts(parametersToGet, allParameters);
		}
		microinvertersToGetIterator++;
	}
}