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

	virtual void setValue(uint16_t *readArray, int registerCount);

	  public:
	PortParameter(std::string name, uint16_t addressOffset, int registerSize);

	std::string name;

	int age;

	virtual void updateValue(modbus_t *modbus_context, uint16_t portStartAddress);
};



template<typename Type>
class PortParameterTemplate : virtual public PortParameter {
	  public:
	PortParameterTemplate(std::string name, uint16_t addressOffset, int registerSize);

	Type value;

	Type getValue();
};

class PortParameterInt : public PortParameterTemplate<int> {
	  private:
	void setValue(uint16_t *readArray, int registerCount);

      public:
	PortParameterInt();
};



class PortParameterSerialNumber : public PortParameterTemplate<std::string> {
      private:
    void setValue(uint16_t *readArray, int registerCount);

      public:
	PortParameterSerialNumber();
};



class PortParameterFloat : public PortParameterTemplate<float> {
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

	std::shared_ptr<PortParameter> getParameterById(int i);

	std::shared_ptr<PortParameter> getParameterByName(std::string name);
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