#ifndef PORTPARAMETERSGENERIC_H
#define PORTPARAMETERSGENERIC_H

#include <stdint.h>
#include <string>
#include <memory>
#include <mutex>

struct _modbus;
typedef _modbus modbus_t;

class PortParameter {
      protected:
	uint16_t parameterAddressOffset;
	int registerSize;

	virtual void setValueFromRegisters(uint16_t *readArray, int readArraySize);

      public:
	PortParameter(std::string name, uint16_t parameterAddressOffset, int registerSize);

	virtual ~PortParameter();

	enum PortParameterValueType { Int, Float };

	union PortParameterValue {
		long i;
		float f;
	};

      protected:
    PortParameterValueType valueType;
	PortParameterValue value;

      public:
	std::string name;
	int age;

	std::pair<PortParameterValue, PortParameterValueType> getValue();

	virtual std::string getOutputValue();

	void updateValue(uint16_t *readArray, int readArraySize, int registerCount);
};

class PortParameterFloat : virtual public PortParameter {
      protected:
    int decimalPlaces;

    virtual void setValueFromRegisters(uint16_t *readArray, int readArraySize);

	  public:
	PortParameterFloat(std::string name, int decimalPlaces, uint16_t parameterAddressOffset, int registerSize);

	std::string getOutputValue();
};

class PortParameterInt : virtual public PortParameter {
	  protected:
	virtual void setValueFromRegisters(uint16_t *readArray, int readArraySize);

	  public:
	PortParameterInt(std::string name, uint16_t parameterAddressOffset, int registerSize);

	std::string getOutputValue();
};

#endif