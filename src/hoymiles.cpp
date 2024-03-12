#include "hoymiles.h"
#include "modbus.h"

Dtu::Dtu(const char *ip_address, int port){
    this->modbus_t = modbus_new_tcp(ip_address, port);
    modbus_connect(this->modbus_t);
}

void Dtu::readTest(){
    uint16_t tempArray[32];
    modbus_read_registers(this->modbus_t, 0x13ef, 1, tempArray);
}

Dtu::~Dtu(){
    delete this->modbus_t;
}