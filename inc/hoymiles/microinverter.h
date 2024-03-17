#ifndef MICROINVERTER_H
#define MICROINVERTER_H

#include <vector>
#include <memory>
#include <mutex>

#include "port.h"

struct _modbus;
typedef _modbus modbus_t;

class Microinverter {
      private:
	std::shared_ptr<modbus_t*> modbus_context;
	std::mutex *modbus_context_mutex;

	std::vector<Port> ports;

	void populatePorts();

      public:
	Microinverter(std::shared_ptr<modbus_t*> modbus_context, std::mutex *modbus_context_mutex);

	void updatePorts();

	void updatePort(int i);

	Port getPort(int i);
};

#endif