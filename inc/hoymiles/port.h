#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#include "portParametersGeneric.h"

struct _modbus;
typedef _modbus modbus_t;

class Port {
      private:
	modbus_t *modbus_context;
	uint16_t portStartAddress;

	std::vector<std::shared_ptr<PortParameter>> parameters;

	void populateParameters();

      public:
	Port(modbus_t *modbus_context, uint16_t portStartAddress);

	void updateParameters();
};

#endif