#include "sunspecParametersGeneric.h"

SunspecParameter::~SunspecParameter() {}

SunspecParameter::SunspecParameter(std::string name, int registerAddressOffset, int registerSize) {
    this->name = name;
    this->registerAddressOffset = registerAddressOffset;
    this->registerSize = registerSize;
}

void SunspecParameter::setValueFromRegisters(uint16_t *registers, int addressOffset) {}

std::pair<SunspecParameter::SunspecValue, SunspecParameter::SunspecValueType> SunspecParameter::getValue() {}

SunspecParameterString32::SunspecParameterString32(std::string name, int registerAddressOffset, int registerSize) : SunspecParameter(name, registerAddressOffset, registerSize) {} 

void SunspecParameterString32::setValueFromRegisters(uint16_t *registers, int addressOffset) {}