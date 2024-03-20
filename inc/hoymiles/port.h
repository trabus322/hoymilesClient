#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <string>
#include <vector>

#include "portParametersGeneric.h"
#include "modbus.h"

class Port {
      private:
	std::shared_ptr<class modbus> modbus;

	uint16_t portStartAddress;

	void populateParameters();

	void fixCurrent();
	bool currentFixed;

      public:
	Port(std::shared_ptr<class modbus> modbus, uint16_t portStartAddress);

	std::vector<std::shared_ptr<PortParameter>> parameters;

	std::pair<std::shared_ptr<PortParameter>, bool> getParameterByName(std::string name);

	void updateParameters(std::vector<std::string> &parametersToGet, bool allParameters);

	void printParameters(std::vector<std::string> &parametersToGet, bool allParameters);
};

#endif