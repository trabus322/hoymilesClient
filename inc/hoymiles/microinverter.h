#ifndef MICROINVERTER_H
#define MICROINVERTER_H

#include <memory>
#include <string>
#include <vector>

#include "modbus.h"
#include "port.h"

class Microinverter {
      private:
	std::shared_ptr<class modbus> modbus;

	int startAddress;

      public:
	Microinverter(std::shared_ptr<class modbus> modbus, int startAddress, long long serialNumber);

	long long serialNumber;

	int age;

	std::vector<Port> ports;

	// void updatePorts(std::vector<std::string> &parametersToGet, bool allParameters);

	void updateParameters(std::vector<std::string> &parametersToGet, bool allParameters);

	void printPorts(std::vector<std::string> &parametersToGet, bool allParameters, bool shortNames);

	long long getTodayProduction();

	long long getTotalProduction();
};

#endif