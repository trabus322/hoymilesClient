#ifndef DTU_H
#define DTU_H

#include <vector>

#include "microinverter.h"

struct _modbus;
typedef _modbus modbus_t;

class Dtu {
      private:
	modbus_t *modbus_context;

	std::vector<Microinverter> microinverters;

	void populateMicroinverters();

      public:
	Dtu(const char *ip_address, int port);

	void updateMicroinverters();

	~Dtu();
};

#endif