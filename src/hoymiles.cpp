#include "hoymiles.h"
#include "modbus.h"

#include <iostream>

Dtu::Dtu(const char *ip_address, int port) {
	this->modbus_t = modbus_new_tcp(ip_address, port);
    if(modbus_connect(this->modbus_t) == -1){
        std::cerr << "conn_error";
        modbus_free(this->modbus_t);
        abort();
    }
}

void Dtu::readTest(uint16_t address, int registers) {
	uint16_t readArray[registers];
    int registerCount;
	registerCount = modbus_read_registers(this->modbus_t, address, registers, readArray);
	if(registerCount == -1){
        std::cerr << "read_error";
        return;
    }
    for (int i{0}; i < registerCount; i++) {
		std::clog << readArray[i] << std::endl;
	}
}

Dtu::~Dtu() {
	modbus_close(this->modbus_t);
	modbus_free(this->modbus_t);
}