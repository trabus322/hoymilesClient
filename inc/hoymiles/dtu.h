#ifndef DTU_H
#define DTU_H

#include <memory>
#include <string>
#include <vector>

#include "microinverter.h"
#include "modbus.h"

class Dtu {
      private:
	modbus_t *modbus;

	std::vector<Microinverter> microinverters;

	Port dtuPort;

	int rtuId;

	bool connected;

	void populateMicroinverters();

      public:
	Dtu(const char *address, int id, bool rtu, bool tcp);

	Dtu(modbus_t *modbus, int id);

	std::pair<Microinverter *, bool> getMicroinverterBySerialNumber(long long serialNumber);

	bool isConnected();

	void updateMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet);

	void printMicroinverters(std::vector<std::string> &parametersToGet, bool allParameters, std::vector<long long> &microinvertersToGet, bool shortNames, bool printTodayProduction, bool printTotalProduction);

	void setStatusMicroinverters(uint16_t value, std::string statusName, std::vector<long long>& microinvertersToSet);

	bool empty();

	void listOfMicroinverters();

	float getCurrentPower();

	int getCurrentOnOff();

	void turnOffMicroinverters();
	void turnOnMicroinverters();

	void limitMicroinverters(uint16_t limit);

	~Dtu();
};

#endif