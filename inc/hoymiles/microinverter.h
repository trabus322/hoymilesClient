#ifndef MICROINVERTER_H
#define MICROINVERTER_H

#include <memory>
#include <string>
#include <vector>

#include "modbus.h"
#include "port.h"
#include "sunspec.h"

class Microinverter {
      private:
	modbus_t *modbus;

	// Sunspec sunspec;

	int startAddress;

	int statusStartAddress;

      public:
	Microinverter(modbus_t *modbus, int startAddress, long long serialNumber);

	long long serialNumber;

	int age;

	std::vector<Port> ports;

	// void updatePorts(std::vector<std::string> &parametersToGet, bool allParameters);

	void updateParameters(std::vector<std::string> &parametersToGet, bool allParameters);

	void updateStatusParameters();

	void printPorts(std::vector<std::string> &parametersToGet, bool allParameters, bool shortNames);

	long long getTodayProduction();

	long long getTotalProduction();

	void setStatus(std::vector<std::pair<int, uint16_t>> portsToSet, std::string statusName);

	void setStatusWholeMicroinverter(uint16_t value, std::string statusName);

	float getCurrentPower();

	int getCurrentOnOff();
};

#endif