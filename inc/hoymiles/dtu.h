#ifndef DTU_H
#define DTU_H

#include <vector>
#include <memory>
#include <string>
// #include <mutex>

#include "microinverter.h"
#include "modbus.h"

// struct _modbus;
// typedef _modbus modbus_t;

class Dtu {
      private:
	// std::shared_ptr<modbus_t*> modbus_context;

	std::shared_ptr<class modbus> modbus;

	// std::mutex modbus_context_mutex;

	std::vector<Microinverter> microinverters;

	bool connected;

	void populateMicroinverters();

	std::pair<bool, Microinverter*> getMicroinverterBySerialNumber(long serialNumber);

      public:
	Dtu(const char *ip_address, int port);

	bool isConnected();

	void updateMicroinverters();

	void updateMicroinverters(std::vector<std::string> &parametersToGet);

	void printMicroinverters();

	void printMicroinverters(std::vector<std::string> &parametersToGet);

	~Dtu();
};

#endif