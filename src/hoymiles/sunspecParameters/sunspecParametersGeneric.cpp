#include "sunspecParametersGeneric.h"

SunspecParameter::~SunspecParameter() {}

SunspecParameter::SunspecParameter(std::string name, int registerAddressOffset, int registerSize) {
    this->name = name;
    this->registerAddressOffset = registerAddressOffset;
    this->registerSize = registerSize;
}

void SunspecParameter::getValueFromRegisters(uint16_t *registers, int addressOffset) {}

std::pair<SunspecParameter::SunspecValue, SunspecParameter::SunspecValueType> SunspecParameter::getValue() {
    return {this->value, this->valueType};
}

SunspecParameterString32::SunspecParameterString32(std::string name, int registerAddressOffset, int registerSize) : SunspecParameter(name, registerAddressOffset, registerSize) {} 

void SunspecParameterString32::getValueFromRegisters(uint16_t *registers, int addressOffset) {}