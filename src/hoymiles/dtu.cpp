#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "modbus.h"

#include "dtu.h"
#include "microinverter.h"

#include "portParameters.h"

Dtu::Dtu(const char *address, int id, bool rtu, bool tcp) : dtuPort(0) {
	dtuPort.statusPortStartAddress = 0xd000;
	if (tcp) {
		this->modbus = modbus_new_tcp(address, id);
	}
	if (rtu) {
		this->modbus = modbus_new_rtu(address, 9600, 'N', 8, 1);
		modbus_rtu_set_serial_mode(this->modbus, MODBUS_RTU_RS485);
	}

	this->connected = false;
	if (modbus_connect(this->modbus) == -1) {
		std::cerr << "[" << id << "] NOT_CONNECTED" << std::endl;
	} else {
		this->connected = true;
		if (rtu) {
			modbus_set_slave(this->modbus, id);
		}
		this->populateMicroinverters();
	}
}

Dtu::Dtu(modbus_t *modbus, int id = -1) : dtuPort(0) {
	dtuPort.statusPortStartAddress = 0xd000;
	
	this->modbus = modbus;
	this->rtuId = id;

	this->connected = false;
	if (modbus_connect(this->modbus) == -1) {
		std::cerr << "[" << id << "] NOT_CONNECTED" << std::endl;
	} else {
		this->connected = true;
		if (id != -1) {
			modbus_set_slave(this->modbus, rtuId);
		}
		this->populateMicroinverters();
	}
}

bool Dtu::isConnected() { return this->connected; }

Dtu::~Dtu() {
	modbus_close(this->modbus);
	modbus_free(this->modbus);
}

void Dtu::populateMicroinverters() {
	if(this->rtuId != -1) {
		modbus_set_slave(this->modbus, this->rtuId);
	}

	int portStartAddress = 0x4000;
	int registersToRead{19};
	uint16_t registers[registersToRead];

	bool lastSuccesful{true};

	int addressToSkip{-1};

	while (portStartAddress <= (0x4000 + (0x0019 * 99))) {
		int registerCount{-1};
		int timesTried{0};
		while (((timesTried < 3) && (lastSuccesful && registerCount == -1) && portStartAddress != addressToSkip)) {
			registerCount = modbus_read_registers(this->modbus, portStartAddress, registersToRead, registers);
			timesTried++;
		}
		portStartAddress += 0x0019;
		if ((registers[0] == 12 && registerCount != -1) && portStartAddress != addressToSkip) {
			if (!lastSuccesful) {
				addressToSkip = portStartAddress;
				portStartAddress -= 2 * 0x0019;
			}
			lastSuccesful = true;
			Port port{portStartAddress};
			port.setParametersFromMicroinverterArray(registers, 0);

			if (!this->getMicroinverterBySerialNumber(port.getParameterByName("microinverterSerialNumber").first.get()->getValue().first.i).second) {
				Microinverter microinverter{this->modbus, portStartAddress, port.getParameterByName("microinverterSerialNumber").first.get()->getValue().first.i};
				this->microinverters.push_back(microinverter);
			}

			Microinverter *microinverter = this->getMicroinverterBySerialNumber(port.getParameterByName("microinverterSerialNumber").first.get()->getValue().first.i).first;
			std::vector<Port>::iterator portsIterator = microinverter->ports.begin();
			bool valueExists{false};
			while (portsIterator != microinverter->ports.end() && !valueExists) {
				if (portsIterator->getParameterByName("portNumber").first.get()->getValue().first.i == port.getParameterByName("portNumber").first.get()->getValue().first.i) {
					valueExists = true;
				}
				portsIterator++;
			}
			if (!valueExists) {
				microinverter->ports.push_back(port);
			}
		} else {
			lastSuccesful = false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

std::pair<Microinverter *, bool> Dtu::getMicroinverterBySerialNumber(long long serialNumber) {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	while (microinvertersIterator != this->microinverters.end()) {
		if (microinvertersIterator->serialNumber == serialNumber) {
			return std::pair<Microinverter *, bool>(&*microinvertersIterator, true);
		} else {
			microinvertersIterator++;
		}
	}
	return std::pair<Microinverter *, bool>(&*microinvertersIterator, false);
}

void Dtu::updateMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet) {
	if(this->rtuId != -1) {
		modbus_set_slave(this->modbus, this->rtuId);
	}
	if (microinvertersToGet.empty()) {
		std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
		while (microinvertersIterator != this->microinverters.end()) {
			microinvertersToGet.push_back(microinvertersIterator->serialNumber);
			microinvertersIterator++;
		}
	}

	std::vector<long long>::iterator microinvertersToGetIterator = microinvertersToGet.begin();
	while (microinvertersToGetIterator != microinvertersToGet.end()) {
		std::pair<Microinverter *, bool> microinverterPair = this->getMicroinverterBySerialNumber(*microinvertersToGetIterator);
		if (microinverterPair.second) {
			microinverterPair.first->updateParameters(parametersToGet, allParameters);
			microinverterPair.first->updateStatusParameters();
		}
		microinvertersToGetIterator++;
	}
}

void Dtu::printMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet, bool shortNames, bool printTodayProduction, bool printTotalProduction) {
	if (microinvertersToGet.empty()) {
		std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
		while (microinvertersIterator != this->microinverters.end()) {
			microinvertersToGet.push_back(microinvertersIterator->serialNumber);
			microinvertersIterator++;
		}
	}

	std::vector<long long>::iterator microinvertersToGetIterator = microinvertersToGet.begin();
	while (microinvertersToGetIterator != microinvertersToGet.end()) {
		std::pair<Microinverter *, bool> microinverterPair = this->getMicroinverterBySerialNumber(*microinvertersToGetIterator);
		if (microinverterPair.second) {
			std::cout << "  "
				  << "Microinverter: " << microinverterPair.first->serialNumber << std::endl;
			std::cout << "  "
				  << "Microinverter Data Age: " << microinverterPair.first->age << std::endl;
			if (printTodayProduction) {
				std::cout << "  "
					  << "TodayProduction: " << microinverterPair.first->getTodayProduction() << "Wh" << std::endl;
			}
			if (printTotalProduction) {
				std::cout << "  "
					  << "TotalProduction: " << microinverterPair.first->getTotalProduction() << "Wh" << std::endl;
			}
			microinverterPair.first->printPorts(parametersToGet, allParameters, shortNames);
			std::cout << std::endl;
		}
		microinvertersToGetIterator++;
	}
}

void Dtu::setStatusMicroinverters(uint16_t value, std::string statusName, std::vector<long long> &microinvertersToSet) {
	if (microinvertersToSet.empty()) {
		std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
		while (microinvertersIterator != this->microinverters.end()) {
			microinvertersToSet.push_back(microinvertersIterator->serialNumber);
			microinvertersIterator++;
		}
	}

	std::vector<long long>::iterator microinvertersToSetIterator = microinvertersToSet.begin();
	while (microinvertersToSetIterator != microinvertersToSet.end()) {
		std::pair<Microinverter *, bool> microinverterPair = this->getMicroinverterBySerialNumber(*microinvertersToSetIterator);
		if (microinverterPair.second) {
			microinverterPair.first->setStatusWholeMicroinverter(value, statusName);
		}
		microinvertersToSetIterator++;
	}
}

bool Dtu::empty() { return this->microinverters.empty(); }

void Dtu::listOfMicroinverters() {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	std::cout << "Microinverter list:" << std::endl;
	while (microinvertersIterator != this->microinverters.end()) {
		std::cout << "  " << microinvertersIterator->serialNumber << std::endl;
		microinvertersIterator++;
	}
}

float Dtu::getCurrentPower() {
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	float currentPower{0};
	while (microinvertersIterator != this->microinverters.end()) {
		currentPower += microinvertersIterator->getCurrentPower();
		microinvertersIterator++;
	}
	return currentPower;
}

int Dtu::getCurrentOnOff() {
	if(this->microinverters.size() == 0) {
		return 0;
	}
	std::vector<Microinverter>::iterator microinvertersIterator = this->microinverters.begin();
	int currentOn{0};
	while(microinvertersIterator != this->microinverters.end()) {
		currentOn += microinvertersIterator->getCurrentOnOff();
		microinvertersIterator++;
	}
	float balance = currentOn / this->microinverters.size();
	if(balance >= 0.5) {
		return 1;
	}
	else {
		return 0;
	}
}

void Dtu::turnOffMicroinverters() { this->dtuPort.getStatusByName("onOff").first.get()->writeValue(0, this->modbus, this->dtuPort.statusPortStartAddress); }

void Dtu::turnOnMicroinverters() { this->dtuPort.getStatusByName("onOff").first.get()->writeValue(1, this->modbus, this->dtuPort.statusPortStartAddress); }

void Dtu::limitMicroinverters(uint16_t limit) { this->dtuPort.getStatusByName("limitActivePower").first.get()->writeValue(limit, this->modbus, this->dtuPort.statusPortStartAddress); }