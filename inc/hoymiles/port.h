#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

struct _modbus;
typedef _modbus modbus_t;

class PortParameter {
      protected:
	uint16_t parameterAddressOffset;
	int registerSize;

	virtual void setValueFromRegisters(uint16_t *readArray, int registerCount);

      public:
	PortParameter(std::string name, uint16_t parameterAddressOffset, int registerSize);

	virtual ~PortParameter();

	enum PortParameterValueType { Int, Float };

	union PortParameterValue {
		int i;
		float f;
	};

      protected:
    PortParameterValueType valueType;
	PortParameterValue value;

      public:
	std::string name;

	std::pair<PortParameterValue, PortParameterValueType> getValue();

	virtual std::string getOutputValue();

	void updateValue(modbus_t *modbus_context, uint16_t portStartAddress);
};

class PortParameterFloat : public PortParameter {
      protected:
    int decimalPlaces;

    void setValueFromRegisters(uint16_t *readArray, int registerCount);

	  public:
	PortParameterFloat(std::string name, int decimalPlaces, uint16_t parameterAddressOffset, int registerSize);

	virtual std::string getOutputValue();
};

class PortParameterInt : public PortParameter {
	  protected:
	void setValueFromRegisters()
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

#endif