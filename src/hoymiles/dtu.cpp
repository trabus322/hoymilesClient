#include <vector>
#include <iostream>
// #include <thread>

#include "modbus.h"

#include "dtu.h"
#include "microinverter.h"

#include "portParameters.h"

// struct _modbus;
// typedef _modbus modbus_t;

Dtu::Dtu(const char *ip_address, int port) {
	class modbus modbus{ip_address, (uint16_t) port};
	this->modbus = std::make_shared<class modbus>(modbus);

	if (!this->modbus.get()->modbus_connect()) {
		std::cerr << "conn_error";
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

		// this->modbus_context_mutex.lock();
		registerCount = this->modbus.get()->modbus_read_holding_registers(portStartAddress + 0x0021, 1, readArray);
		// this->modbus_context_mutex.unlock();
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

void Dtu::updateMicroinverters() {
	// std::vector<std::thread> updateThreads;

	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while (microinvertersIterator != this->microinverters.end()) {
		// updateThreads.push_back(std::thread(&Microinverter::updatePorts, microinvertersIterator));
		microinvertersIterator->updatePorts();
		microinvertersIterator++;
	}

	// std::vector<std::thread>::iterator updateThreadsIterator = updateThreads.begin();
	// while(updateThreadsIterator != updateThreads.end()) {
	// 	updateThreadsIterator->join();		updateThreadsIterator++;
	// }
	// std::cout << std::endl;
}

void Dtu::printMicroinverters() {
	std::cout << "DTU:" << std::endl;
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while(microinvertersIterator != this->microinverters.end()) {
		microinvertersIterator->printPorts();
		std::cout << std::endl;
		microinvertersIterator++;
	}
}