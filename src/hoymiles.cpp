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
	std::vector<Microinverter>::iterator microinvertersIterator{this->microinverters.begin()};
	std::vector<std::thread> updateThreads;
	while (microinvertersIterator != this->microinverters.end()) {
		updateThreads.push_back(std::thread{&Microinverter::updateValues, *microinvertersIterator});
		microinvertersIterator++;
	}
	std::vector<std::thread>::iterator updateThreadsIterator{updateThreads.begin()};
	while (updateThreadsIterator != updateThreads.end()) {
		updateThreadsIterator->join();
		updateThreadsIterator++;
	}
	int numberOfInverters = this->microinverters.size();
}

Microinverter::Microinverter(modbus_t *modbus_context, uint16_t address) {
	this->modbus_context = modbus_context;
	this->address = address;

	uint16_t serialNumberReadArray[5];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 2, 5, serialNumberReadArray);

	this->serialNumber = 0;
	for (int i{0}; i < registerCount; i++) {
		serialNumber += serialNumberReadArray[i];
	}
}

void Microinverter::updateValues() { 
    std::thread updatePortNumberThread{&Microinverter::updatePortNumber, this};

    std::thread updatePlantVoltageThread{&Microinverter::updatePlantVoltage, this};
    std::thread updatePlantCurrentThread{&Microinverter::updatePlantCurrent, this};

    std::thread updateGridVoltageThread{&Microinverter::updateGridVoltage, this};
    std::thread updateGridFrequencyThread{&Microinverter::updateGridFrequency, this};

    std::thread updatePlantPowerThread{&Microinverter::updatePlantPower, this};

    std::thread updateTodayProductionThread{&Microinverter::updateTodayProduction, this};
    std::thread updateTotalProductionThread{&Microinverter::updateTotalProduction, this};

    std::thread updateTemperatureThread{&Microinverter::updateTemperature, this};

    std::thread updateOperatingStatus{&Microinverter::updateOperatingStatus, this};
    std::thread updateAlarmCodeThread{&Microinverter::updateAlarmCode, this};
    std::thread updateAlarmCountThread{&Microinverter::updateAlarmCount, this};

    std::thread updateLinkStatusThread{&Microinverter::updateLinkStatus, this};


    updatePortNumberThread.join();

    updatePlantVoltageThread.join();
    updatePlantCurrentThread.join();

    updateGridVoltageThread.join();
    updateGridFrequencyThread.join();

    updatePlantPowerThread.join();

    updateTodayProductionThread.join();
    updateTotalProductionThread.join();

    updateTemperatureThread.join();

    updateOperatingStatus.join();
    updateAlarmCodeThread.join();
    updateAlarmCountThread.join();

    updateLinkStatusThread.join();
}

void Microinverter::updatePortNumber() {
	uint16_t readArray[1];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 7, 1, readArray);
	if (registerCount == -1) {
		this->portNumber.second++;
	} else {
		this->portNumber.first = readArray[0];
		this->portNumber.second = 0;
	}    
}

void Microinverter::updatePlantVoltage() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 8, 2, readArray);
	if (registerCount == -1) {
		this->plantVoltage.second++;
	} else {
		this->plantVoltage.first = readArray[0];
		this->plantVoltage.first += readArray[1];
		this->plantVoltage.second = 0;
	}
}

void Microinverter::updatePlantCurrent() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 10, 2, readArray);
	if (registerCount == -1) {
		this->plantCurrent.second++;
	} else {
		this->plantCurrent.first = readArray[0];
		this->plantCurrent.first += readArray[1];
		this->plantCurrent.second = 0;
	}
}

void Microinverter::updateGridVoltage() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 12, 2, readArray);
	if (registerCount == -1) {
		this->gridVoltage.second++;
	} else {
		this->gridVoltage.first = readArray[0];
		this->gridVoltage.first += readArray[1];
		this->gridVoltage.second = 0;
	}
}

void Microinverter::updateGridFrequency() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 14, 2, readArray);
	if (registerCount == -1) {
		this->gridFrequency.second++;
	} else {
		this->gridFrequency.first = readArray[0];
		this->gridFrequency.first += readArray[1];
		this->gridFrequency.second = 0;
	}
}

void Microinverter::updatePlantPower() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 16, 2, readArray);
	if (registerCount == -1) {
		this->plantPower.second++;
	} else {
		this->plantPower.first = readArray[0];
		this->plantPower.first += readArray[1];
		this->plantPower.second = 0;
	}
}

void Microinverter::updateTodayProduction() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 18, 2, readArray);
	if (registerCount == -1) {
		this->todayProduction.second++;
	} else {
		this->todayProduction.first = readArray[0];
		this->todayProduction.first += readArray[1];
		this->todayProduction.second = 0;
	}
}

void Microinverter::updateTotalProduction() {
	uint16_t readArray[4];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 20, 4, readArray);
	if (registerCount == -1) {
		this->totalProduction.second++;
	} else {
		this->totalProduction.first = readArray[0];
		this->totalProduction.first += readArray[1];
        this->totalProduction.first += readArray[2];
        this->totalProduction.first += readArray[3];
		this->totalProduction.second = 0;
	}
}

void Microinverter::updateTemperature() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 24, 2, readArray);
	if (registerCount == -1) {
		this->temperature.second++;
	} else {
		this->temperature.first = readArray[0];
		this->temperature.first += readArray[1];
		this->temperature.second = 0;
	}
}

void Microinverter::updateOperatingStatus() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 26, 2, readArray);
	if (registerCount == -1) {
		this->operatingStatus.second++;
	} else {
		this->operatingStatus.first = readArray[0];
		this->operatingStatus.first += readArray[1];
		this->operatingStatus.second = 0;
	}
}

void Microinverter::updateAlarmCode() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 28, 2, readArray);
	if (registerCount == -1) {
		this->alarmCode.second++;
	} else {
		this->alarmCode.first = readArray[0];
		this->alarmCode.first += readArray[1];
		this->alarmCode.second = 0;
	}
}

void Microinverter::updateAlarmCount() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 30, 2, readArray);
	if (registerCount == -1) {
		this->alarmCount.second++;
	} else {
		this->alarmCount.first = readArray[0];
		this->alarmCount.first += readArray[1];
		this->alarmCount.second = 0;
	}
}

void Microinverter::updateLinkStatus() {
	uint16_t readArray[2];
	int registerCount;
	registerCount = modbus_read_registers(this->modbus_context, this->address + 32, 2, readArray);
	if (registerCount == -1) {
		this->linkStatus.second++;
	} else {
		this->linkStatus.first = readArray[0];
		this->linkStatus.first += readArray[1];
		this->linkStatus.second = 0;
	}
}