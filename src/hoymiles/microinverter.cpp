#include <thread>

#include "modbus.h"

#include "microinverter.h"
#include "port.h"

struct _modbus;
typedef _modbus modbus_t;

Microinverter::Microinverter(std::shared_ptr<modbus_t*> modbus_context, std::mutex *modbus_context_mutex) {
	this->modbus_context = modbus_context;
	this->modbus_context_mutex = modbus_context_mutex;

	this->populatePorts();
}

void Microinverter::populatePorts() {
	uint16_t portStartAddress = 0x1000;
	uint16_t readArray[1];

	int registerCount;
	registerCount = modbus_read_registers(*this->modbus_context.get(), portStartAddress + 0x0021, 1, readArray);
	while(registerCount != -1 && readArray[0] == 0x700) {
		Port port{ this->modbus_context, this->modbus_context_mutex, portStartAddress };
		this->ports.push_back(port);
		portStartAddress += 0x0028;

		this->modbus_context_mutex->lock();
		registerCount = modbus_read_registers(*this->modbus_context.get(), portStartAddress + 0x0021, 1, readArray);
		this->modbus_context_mutex->unlock();

		break;
	}
}

void Microinverter::updatePorts() {
	std::vector<std::thread> updateThreads;
	for(Port port : this->ports){
		updateThreads.push_back(std::thread(&Port::updateParameters, port));
		// port.updateParameters();
	}
	std::vector<std::thread>::iterator updateThreadsIterator = updateThreads.begin();
	while(updateThreadsIterator != updateThreads.end()) {
		updateThreadsIterator->join();
		updateThreadsIterator++;
	}
}