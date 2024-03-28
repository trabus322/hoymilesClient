#include <iostream>
#include <string>
#include <vector>

#include "modbus.h"

#include "dtu.h"
#include "microinverter.h"

#include "portParameters.h"

Dtu::Dtu(const char *ip_address, int port) {
	class modbus modbus {
		ip_address, (uint16_t)port
	};
	this->modbus = std::make_shared<class modbus>(modbus);

	if (!this->modbus.get()->modbus_connect()) {
		std::cerr << "NOT CONNECTED" << std::endl;
	}

	if (this->modbus.get()->is_connected()) {
		this->populateMicroinverters();
	}
}

bool Dtu::isConnected() { return this->modbus.get()->is_connected(); }

bool Dtu::modbusError() { return this->modbus.get()->err; }

std::string Dtu::modbusErrorMessage() { return this->modbus.get()->error_msg; }

Dtu::~Dtu() { this->modbus.get()->modbus_close(); }

void Dtu::populateMicroinverters() {
	int portStartAddress = 0x1000;
	uint16_t readArrayJoined[20];
	uint8_t registers[40];

	int registerCount;
	registerCount = this->modbus.get()->modbus_read_holding_registers(portStartAddress, 20, readArrayJoined);

	if (registerCount != 0) {
		return;
	}

	while (registerCount == 0) {
		for (int i{0}; i < 20; i ++) {
			registers[2 * i] = (readArrayJoined[i] & 0xFF00) >> 8;
			registers[(2 * i) + 1] = (readArrayJoined[i] & 0x00FF);
		}

		if(registers[0] != 12) {
			break;
		}

		Port port{portStartAddress};
		port.setParametersFromMicroinverterArray(registers, 0);

		if (!this->getMicroinverterBySerialNumber(port.getParameterByName("microinverterSerialNumber").first.get()->getValue().first.i).second) {
			Microinverter microinverter{this->modbus, portStartAddress, port.getParameterByName("microinverterSerialNumber").first.get()->getValue().first.i};
			this->microinverters.push_back(microinverter);
		}

		this->getMicroinverterBySerialNumber(port.getParameterByName("microinverterSerialNumber").first.get()->getValue().first.i).first->ports.push_back(port);

		portStartAddress += 0x0028;

		registerCount = this->modbus.get()->modbus_read_holding_registers(portStartAddress, 20, readArrayJoined);
	}
}

std::pair<Microinverter *, bool> Dtu::getMicroinverterBySerialNumber(long long serialNumber) {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while (microinvertersIterator != this->microinverters.end()) {
		if (microinvertersIterator->serialNumber == serialNumber) {
			return std::pair<Microinverter *, bool>(&*microinvertersIterator, true);
		} else {
			microinvertersIterator++;
		}
	}
	return std::pair<Microinverter *, bool>(&*microinvertersIterator, false);
}

void Dtu::updateMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet) {
	if (microinvertersToGet.empty()) {
		std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
		while (microinvertersIterator != this->microinverters.end()) {
			microinvertersToGet.push_back(microinvertersIterator->serialNumber);
			microinvertersIterator++;
		}
	}

	std::vector<long long>::iterator microinvertersToGetIterator = microinvertersToGet.begin();
	while (microinvertersToGetIterator != microinvertersToGet.end()) {
		std::pair<Microinverter *, bool> microinverterPair = this->getMicroinverterBySerialNumber(*microinvertersToGetIterator);
		if (microinverterPair.second) {
			microinverterPair.first->updateParameters(parametersToGet, allParameters);
		}
		microinvertersToGetIterator++;
	}
}

void Dtu::printMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet, bool shortNames, bool printTodayProduction, bool printTotalProduction) {
	if (microinvertersToGet.empty()) {
		std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
		while (microinvertersIterator != this->microinverters.end()) {
			microinvertersToGet.push_back(microinvertersIterator->serialNumber);
			microinvertersIterator++;
		}
	}

	std::vector<long long>::iterator microinvertersToGetIterator = microinvertersToGet.begin();
	while (microinvertersToGetIterator != microinvertersToGet.end()) {
		std::pair<Microinverter *, bool> microinverterPair = this->getMicroinverterBySerialNumber(*microinvertersToGetIterator);
		if (microinverterPair.second) {
			std::cout << "  " << "Microinverter: " << microinverterPair.first->serialNumber << std::endl;
			std::cout << "  " << "Microinverter Data Age: " << microinverterPair.first->age << std::endl;
			if (printTodayProduction) {
				std::cout << "  " << "TodayProduction: " << microinverterPair.first->getTodayProduction() << std::endl;
			}
			if (printTotalProduction) {
				std::cout << "  " << "TotalProduction: " << microinverterPair.first->getTotalProduction() << std::endl;
			}
			microinverterPair.first->printPorts(parametersToGet, allParameters, shortNames);
			std::cout << std::endl;
		}
		microinvertersToGetIterator++;
	}
}