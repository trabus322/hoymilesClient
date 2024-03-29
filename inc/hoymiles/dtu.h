#ifndef DTU_H
#define DTU_H

#include <memory>
#include <string>
#include <vector>

#include "microinverter.h"
#include "modbus.h"

class Dtu {
      private:
	std::shared_ptr<class modbus> modbus;

	std::vector<Microinverter> microinverters;

	bool connected;

	void populateMicroinverters();

	std::pair<Microinverter *, bool> getMicroinverterBySerialNumber(long long serialNumber);

      public:
	Dtu(const char *ip_address, int port);

	bool isConnected();

	// void updateMicroinverters();

	void updateMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet);

	// void printMicroinverters();

	void printMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet, bool shortNames, bool printTodayProduction, bool printTotalProduction);

	~Dtu();
};

#endif