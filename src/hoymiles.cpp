#include "hoymiles.h"
#include "modbus.h"

#include <iostream>
#include <thread>

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
	Microinverter microinverter{this->modbus_context, address};
	this->microinverters.push_back(microinverter);
}

void Dtu::updateMicroinverters() {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while(microinvertersIterator != this->microinverters.end()){
		microinvertersIterator->updateParameters();
		microinvertersIterator++;
	}
}

MicroinverterParameter::MicroinverterParameter(std::string name, uint16_t addressOffset, int registerSize) {
	this->name = name;

	this->addressOffset = addressOffset;
	this->registerSize = registerSize;
}

void MicroinverterParameter::updateValue(modbus_t *modbus_context, uint16_t microinverterAddress) {
	uint16_t readArray[this->registerSize];
	int registerCount;
	registerCount = modbus_read_registers(modbus_context, microinverterAddress + this->addressOffset, this->registerSize, readArray);
	if(registerCount == -1){
		this->age++;
	}
	else{
		for(int i{0}; i<this->registerSize; i++){
			this->valueInt = readArray[i];
		}
		this->age = 0;
	}
}

Microinverter::Microinverter(modbus_t *modbus_t, uint16_t address) {
	this->modbus_context = modbus_t;
	this->address = address;

	MicroinverterParameter microinverterParameter{"gridVoltage", 0x0001, 6};

	this->parameters.push_back(microinverterParameter);
}

void Microinverter::updateParameters() {
	std::vector<MicroinverterParameter>::iterator parametersIterator = this->parameters.begin();
	while(parametersIterator != this->parameters.end()) {
		parametersIterator->updateValue(this->modbus_context, this->address);
		parametersIterator++;
	}
}

void Microinverter::updateParameterByIndex(int i){
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + this->parameters.at(i).addressOffset, this->parameters.at(i).registerSize, readArray);
	if(registerCount == -1){
		this->parameters.at(i).age++;
	}
	else{
		this->parameters.at(i).valueInt = readArray[0];
		this->parameters.at(i).valueInt = readArray[1];
		this->parameters.at(i).age = 0;
	}
}