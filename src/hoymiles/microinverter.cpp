// #include <thread>
#include <iostream>
#include <memory>

#include "modbus.h"

#include "microinverter.h"
#include "port.h"

struct _modbus;
typedef _modbus modbus_t;

Microinverter::Microinverter(std::shared_ptr<modbus_t*> modbus_context, long serialNumber) {
	this->modbus_context = modbus_context;
	// this->modbus_context_mutex = modbus_context_mutex;

	this->serialNumber = serialNumber;
}

void Microinverter::updatePorts() {
	// std::vector<std::thread> updateThreads;
	for(Port port : this->ports){
		// updateThreads.push_back(std::thread(&Port::updateParameters, port));
		port.updateParameters();
	}
	// std::vector<std::thread>::iterator updateThreadsIterator = updateThreads.begin();
	// while(updateThreadsIterator != updateThreads.end()) {
	// 	updateThreadsIterator->join();
	// 	updateThreadsIterator++;
	// }
}

void Microinverter::printPorts() {
	std::cout << "Microinverter: " << this->serialNumber << std::endl;

	std::vector<Port>::iterator portsIterator = this->ports.begin();
	while(portsIterator != this->ports.end()) {
		portsIterator->printParameters();
		std::cout << std::endl;
		portsIterator++;
	}
}