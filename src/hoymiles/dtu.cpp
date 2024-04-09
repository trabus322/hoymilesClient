#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "modbus.h"

#include "dtu.h"
#include "microinverter.h"

#include "portParameters.h"

Dtu::Dtu(const char *address, int id, bool rtu, bool tcp) {
	if(tcp) {
		this->modbus = modbus_new_tcp(address, id);
	}
	if(rtu) {
		this->modbus = modbus_new_rtu(address, 9600, 'N', 8, 1);
		modbus_rtu_set_serial_mode(this->modbus, MODBUS_RTU_RS485);
	}

	this->connected = false;
	if (modbus_connect(this->modbus) == -1) {
		std::cerr << "NOT CONNECTED" << std::endl;
	}
	else {
		this->connected = true;
		if(rtu) {
			modbus_set_slave(this->modbus, id);
		}
		this->populateMicroinverters();
	}
}

bool Dtu::isConnected() { return this->connected; }

Dtu::~Dtu() {
	modbus_close(this->modbus);
	modbus_free(this->modbus);
}

void Dtu::populateMicroinverters() {
	int portStartAddress = 0x4000;
	uint16_t registers[19];

	modbus_set_debug(this->modbus, 1);

	int registerCount;
	registerCount = modbus_read_registers(this->modbus, portStartAddress, 19, registers);

	if (registerCount == -1) {
		return;
	}

	while (portStartAddress <= (0x4000 + 0x0019*99)) {
		// if(registers[0] != 12) {
		// 	break;
		// }

		Port port{portStartAddress};
		port.setParametersFromMicroinverterArray(registers, 0);

		if (!this->getMicroinverterBySerialNumber(port.getParameterByName("microinverterSerialNumber").first.get()->getValue().first.i).second) {
			Microinverter microinverter{this->modbus, portStartAddress, port.getParameterByName("microinverterSerialNumber").first.get()->getValue().first.i};
			this->microinverters.push_back(microinverter);
		}

		this->getMicroinverterBySerialNumber(port.getParameterByName("microinverterSerialNumber").first.get()->getValue().first.i).first->ports.push_back(port);

		portStartAddress += 0x0019;

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		registerCount = modbus_read_registers(this->modbus, portStartAddress, 19, registers);
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
			microinverterPair.first->updateStatusParameters();
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
				std::cout << "  " << "TodayProduction: " << microinverterPair.first->getTodayProduction() << "Wh" << std::endl;
			}
			if (printTotalProduction) {
				std::cout << "  " << "TotalProduction: " << microinverterPair.first->getTotalProduction() << "Wh" << std::endl;
			}
			microinverterPair.first->printPorts(parametersToGet, allParameters, shortNames);
			std::cout << std::endl;
		}
		microinvertersToGetIterator++;
	}
}

void Dtu::setStatusMicroinverters(uint16_t value, std::string statusName, std::vector<long long>& microinvertersToSet) {
	if (microinvertersToSet.empty()) {
		std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
		while (microinvertersIterator != this->microinverters.end()) {
			microinvertersToSet.push_back(microinvertersIterator->serialNumber);
			microinvertersIterator++;
		}
	}

	std::vector<long long>::iterator microinvertersToSetIterator = microinvertersToSet.begin();
	while(microinvertersToSetIterator != microinvertersToSet.end()) {
		std::pair<Microinverter *, bool> microinverterPair = this->getMicroinverterBySerialNumber(*microinvertersToSetIterator);
		if(microinverterPair.second) {
			microinverterPair.first->setStatusWholeMicroinverter(value, statusName);
		}
		microinvertersToSetIterator++;
	}
}

bool Dtu::empty() {
	return this->microinverters.empty();
}

void Dtu::listOfMicroinverters() {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	std::cout << "Microinverter list:" << std::endl;
	while(microinvertersIterator != this->microinverters.end()) {
		std::cout << "  " << microinvertersIterator->serialNumber << std::endl;
		microinvertersIterator++;
	}
}