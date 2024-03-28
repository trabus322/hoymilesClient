#ifndef PORTPARAMETERSGENERIC_H
#define PORTPARAMETERSGENERIC_H

#include <memory>
#include <stdint.h>
#include <string>

struct _modbus;
typedef _modbus modbus_t;

class PortParameter {
      protected:
	uint16_t parameterAddressOffset;
	int registerSize;

      public:
	PortParameter(std::string name, std::string shortName, uint16_t parameterAddressOffset, int registerSize);

	virtual ~PortParameter();

	enum PortParameterValueType { Int, Float };

	union PortParameterValue {
		long long i;
		float f;
	};

      protected:
	PortParameterValueType valueType;
	PortParameterValue value;

      public:
	std::string name;
	std::string shortName;

	// int age;

	std::pair<PortParameterValue, PortParameterValueType> getValue();

	virtual std::string getOutputValue();

	virtual void setValueFromRegisters(uint8_t *registers, int addressOffset);

	// void updateValue(std::shared_ptr<class modbus> modubs, uint16_t portStartAddress);
};

class PortParameterFloat : public PortParameter {
      protected:
	int decimalPlaces;

      public:
	PortParameterFloat(std::string name, std::string shortName, int decimalPlaces, uint16_t parameterAddressOffset, int registerSize);

	std::string getOutputValue();

	virtual void setValueFromRegisters(uint8_t *registers, int addressOffset);
};

class PortParameterInt : public PortParameter {
      protected:

      public:
	PortParameterInt(std::string name, std::string shortName, uint16_t parameterAddressOffset, int registerSize);

	std::string getOutputValue();

	virtual void setValueFromRegisters(uint8_t *registers, int addressOffset);
};

#endif