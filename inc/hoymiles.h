#ifndef HOYMILES_H
#define HOYMILES_H

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

struct _modbus;
typedef _modbus modbus_t;

class PortParameter {
      protected:
	uint16_t addressOffset;

	int registerSize;

	void setValue(uint16_t *readArray, int registerCount);

      public:
	PortParameter(std::string name, uint16_t addressOffset, int registerSize);

	std::string name;
	int age;
	int value;

	virtual void updateValue(modbus_t *modbus_context, uint16_t portStartAddress);
};

class PortParameterSerialNumber : public PortParameter {
      private:
    void setValue(uint16_t *readArray, int registerCount);

      public:
	PortParameterSerialNumber();
};

class PortParameterFloat : public PortParameter {
      private:
	int decimalPlaces;

    void setValue(uint16_t *readArray, int registerCount);

      public:
	PortParameterFloat(std::string name, uint16_t addressOffset, int registerSize, int decimalPlaces);

	float value;
};

class Port {
      private:
	modbus_t *modbus_context;
	uint16_t portStartAddress;

	std::vector<std::shared_ptr<PortParameter>> parameters;

    void populateParameters();

    public:
    Port(modbus_t *modbus_context, uint16_t portStartAddress);

    void updateParameters();
};

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