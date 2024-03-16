#include "microinverter.h"
#include "port.h"

Microinverter::Microinverter(modbus_t *modbus_context) {
	this->modbus_context = modbus_context;

	this->populatePorts();
}

void Microinverter::populatePorts() {
	Port port{this->modbus_context, 0x1000};

	this->ports.push_back(port);
}

void Microinverter::updatePorts() {
	for(Port port : this->ports){
		port.updateParameters();
	}
}