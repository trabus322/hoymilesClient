#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <mutex>

#include "portParametersGeneric.h"

struct _modbus;
typedef _modbus modbus_t;

class Port {
      private:
	std::shared_ptr<modbus_t*> modbus_context;
	std::mutex *modbus_context_mutex;

	uint16_t portStartAddress;

	uint16_t *readArray;
	int readArraySize;

	std::vector<std::shared_ptr<PortParameter>> parameters;

	void populateParameters();

      public:
	Port(std::shared_ptr<modbus_t*> modbus_context, std::mutex *modbus_context_mutex, uint16_t portStartAddress);

	void updateParameters();
};

#endif