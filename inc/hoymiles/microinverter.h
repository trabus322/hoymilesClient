#ifndef MICROINVERTER_H
#define MICROINVERTER_H

#include <vector>

#include "port.h"

struct _modbus;
typedef _modbus modbus_t;

class Microinverter {
      private:
	modbus_t *modbus_context;

	std::vector<Port> ports;

	void populatePorts();

      public:
	Microinverter(modbus_t *modbus_t);

	void updatePorts();

	void updatePort(int i);

	Port getPort(int i);
};

#endif