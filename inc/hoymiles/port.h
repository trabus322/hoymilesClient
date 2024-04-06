#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <string>
#include <vector>

#include "portParametersGeneric.h"
#include "portParameters.h"
#include "modbus.h"

class Port {
      private:
	void populateParameters();

	void fixCurrent();
	bool currentFixed;

	// void increaseParametersAge();

      public:
	Port(int portStartAddress);

	int portStartAddress;

	int statusPortStartAddress;

	std::vector<std::shared_ptr<PortParameter>> parameters;

	std::vector<std::shared_ptr<PortParameter>> statusParameters;

	std::pair<std::shared_ptr<PortParameter>, bool> getParameterByName(std::string name);

	std::pair<std::shared_ptr<PortParameter>, bool> getStatusByName(std::string name);

	// void updateParameters(std::vector<std::string> &parametersToGet, bool allParameters);

	void setParametersFromMicroinverterArray(uint16_t *registers, int addressOffset);

	void setStatusesFromMicroinverterArray(uint16_t *registers, int addressOffset);

	void printParameters(std::vector<std::string> &parametersToGet, bool allParameters, bool shortNames);

	void turnOff(class modbus &modbus);

	bool isOff(class modbus &modbus);
};

#endif