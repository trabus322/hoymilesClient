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

	std::string unit;

	bool r;
	bool w;

      public:
	PortParameter(std::string name, std::string shortName, std::string unit, bool r, bool w, uint16_t parameterAddressOffset, int registerSize);

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

	PortParameter& writeValue(uint16_t value, class modbus& modbus, int portStartAddress);

	virtual std::string getOutputValue();

	virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);

	// void updateValue(std::shared_ptr<class modbus> modubs, uint16_t portStartAddress);
};

class PortParameterFloat : public PortParameter {
      protected:
	int decimalPlaces;

      public:
	PortParameterFloat(std::string name, std::string shortName, std::string unit, bool r, bool w, int decimalPlaces, uint16_t parameterAddressOffset, int registerSize);

	std::string getOutputValue();

	virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
};

class PortParameterInt : public PortParameter {
      protected:

      public:
	PortParameterInt(std::string name, std::string shortName, std::string unit, bool r, bool w, uint16_t parameterAddressOffset, int registerSize);

	std::string getOutputValue();

	virtual void getValueFromRegisters(uint16_t *registers, int addressOffset);
};

#endif