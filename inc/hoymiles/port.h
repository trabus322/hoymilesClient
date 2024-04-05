#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <string>
#include <vector>

#include "portParametersGeneric.h"
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

	std::vector<std::shared_ptr<PortParameter>> parameters;

	std::pair<std::shared_ptr<PortParameter>, bool> getParameterByName(std::string name);

	// void updateParameters(std::vector<std::string> &parametersToGet, bool allParameters);

	void setParametersFromMicroinverterArray(uint16_t *registers, int addressOffset);

	void printParameters(std::vector<std::string> &parametersToGet, bool allParameters, bool shortNames);
};

#endif