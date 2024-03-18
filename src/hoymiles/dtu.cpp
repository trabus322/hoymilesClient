#include <vector>
#include <iostream>
#include <thread>

#include "modbus.h"

#include "dtu.h"
#include "microinverter.h"

#include "portParameters.h"

struct _modbus;
typedef _modbus modbus_t;

Dtu::Dtu(const char *ip_address, int port) {
	this->modbus_context = std::make_shared<modbus_t*>(modbus_new_tcp(ip_address, port));

	if (modbus_connect(*this->modbus_context.get()) == -1) {
		std::cerr << "conn_error";
		modbus_free(*this->modbus_context.get());
		abort();
	}

	this->populateMicroinverters();
}

Dtu::~Dtu() {
	modbus_close(*this->modbus_context.get());
	modbus_free(*this->modbus_context.get());
}

void Dtu::populateMicroinverters() {
	uint16_t portStartAddress = 0x1000;
	uint16_t readArray[1];

	int registerCount;
	registerCount = modbus_read_registers(*this->modbus_context.get(), portStartAddress + 0x0021, 1, readArray);
	while(registerCount != -1 && readArray[0] == 0x700) {
		Port port{ this->modbus_context, &this->modbus_context_mutex, portStartAddress };

		PortParameterMicroinverterSerialNumber portParameterMicroinverterSerialNumber{};
		portParameterMicroinverterSerialNumber.updateValue(this->modbus_context, &this->modbus_context_mutex, portStartAddress);
		long serialNumber = portParameterMicroinverterSerialNumber.getValue().first.i;

		std::pair<bool, Microinverter*> getMicroinverterBySerialNumber = this->getMicroinverterBySerialNumber(serialNumber);
		if(getMicroinverterBySerialNumber.first) {
			getMicroinverterBySerialNumber.second->ports.push_back(port);
		}
		else {
			Microinverter microinverter{ this->modbus_context, &this->modbus_context_mutex, serialNumber };
			this->microinverters.push_back(microinverter);
			this->microinverters.back().ports.push_back(port);
		}

		portStartAddress += 0x0028;

		this->modbus_context_mutex.lock();
		registerCount = modbus_read_registers(*this->modbus_context.get(), portStartAddress + 0x0021, 1, readArray);
		this->modbus_context_mutex.unlock();
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
	std::vector<std::thread> updateThreads;

	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while (microinvertersIterator != this->microinverters.end()) {
		updateThreads.push_back(std::thread(&Microinverter::updatePorts, microinvertersIterator));
		microinvertersIterator++;
	}

	std::vector<std::thread>::iterator updateThreadsIterator = updateThreads.begin();
	while(updateThreadsIterator != updateThreads.end()) {
		updateThreadsIterator->join();		updateThreadsIterator++;
	}
	std::cout << std::endl;
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