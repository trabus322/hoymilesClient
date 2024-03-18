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

	

      public:
	Microinverter(std::shared_ptr<modbus_t*> modbus_context, std::mutex *modbus_context_mutex, long serialNumber);

	long serialNumber;

	std::vector<Port> ports;

	void updatePorts();

	void updatePort(int i);

	Port getPort(int i);
};

#endif