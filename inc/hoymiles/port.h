#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
// #include <mutex>

#include "portParametersGeneric.h"

struct _modbus;
typedef _modbus modbus_t;

class Port {
      private:
	std::shared_ptr<modbus_t*> modbus_context;
	// std::mutex *modbus_context_mutex;

	uint16_t portStartAddress;

	void populateParameters();

	void fixCurrent();
	bool currentFixed;

      public:
	Port(std::shared_ptr<modbus_t*> modbus_context, uint16_t portStartAddress);

	std::vector<std::shared_ptr<PortParameter>> parameters;

	void updateParameters();

	void printParameters();
};

#endif