#include <vector>
#include <iostream>

#include "modbus.h"

#include "dtu.h"
#include "microinverter.h"

struct _modbus;
typedef _modbus modbus_t;

Dtu::Dtu(const char *ip_address, int port) {
	this->modbus_context = modbus_new_tcp(ip_address, port);
	if (modbus_connect(this->modbus_context) == -1) {
		std::cerr << "conn_error";
		modbus_free(this->modbus_context);
		abort();
	}

	this->populateMicroinverters();
}

Dtu::~Dtu() {
	modbus_close(this->modbus_context);
	modbus_free(this->modbus_context);
}

void Dtu::populateMicroinverters() {
	uint16_t address{0x1000};
	Microinverter microinverter{this->modbus_context};
	this->microinverters.push_back(microinverter);
}

void Dtu::updateMicroinverters() {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while (microinvertersIterator != this->microinverters.end()) {
		microinvertersIterator->updatePorts();
		microinvertersIterator++;
	}
}