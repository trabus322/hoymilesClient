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

	void populateMicroinverters();

      public:
	Dtu(const char *ip_address, int port);

	std::pair<Microinverter *, bool> getMicroinverterBySerialNumber(long long serialNumber);

	bool isConnected();

	bool modbusError();

	std::string modbusErrorMessage();

	void updateMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet);

	void printMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet, bool shortNames, bool printTodayProduction, bool printTotalProduction);

	void setStatusMicroinverters(uint16_t value, std::string statusName, std::vector<long long>& microinvertersToSet);

	~Dtu();
};

#endif