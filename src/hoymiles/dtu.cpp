#include <vector>
#include <iostream>

#include "modbus.h"

#include "dtu.h"
#include "microinverter.h"

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
	uint16_t address{0x1000};
	Microinverter microinverter{this->modbus_context, &this->modbus_context_mutex};
	this->microinverters.push_back(microinverter);
}

void Dtu::updateMicroinverters() {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while (microinvertersIterator != this->microinverters.end()) {
		microinvertersIterator->updatePorts();
		microinvertersIterator++;
	}
}