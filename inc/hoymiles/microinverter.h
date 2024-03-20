#ifndef MICROINVERTER_H
#define MICROINVERTER_H

#include <vector>
#include <memory>
#include <string>
// #include <mutex>

#include "port.h"
#include "modbus.h"

// struct _modbus;
// typedef _modbus modbus_t;

class Microinverter {
      private:
	// std::shared_ptr<modbus_t*> modbus_context;

	std::shared_ptr<class modbus> modbus;

	// std::mutex *modbus_context_mutex;

	

      public:
	Microinverter(
	std::shared_ptr<class modbus> modbus, long long serialNumber);

	long long serialNumber;

	std::vector<Port> ports;

	// void updatePorts();

	void updatePorts(std::vector<std::string> &parametersToGet, bool allParameters);

	void updatePort(int i);

	Port getPort(int i);

	// void printPorts();

	void printPorts(std::vector<std::string> &parametersToGet, bool allParameters);
};

#endif