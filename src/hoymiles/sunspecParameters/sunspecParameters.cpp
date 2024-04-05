#include <stdint.h>
#include <string>
#include <sstream>

#include "sunspecParameters.h"

SunspecParameterManufacturer::SunspecParameterManufacturer() : SunspecParameterString32("manufacturer", 4, 16) {
    this->valueType = string32;
}

void SunspecParameterManufacturer::setValueFromRegisters(uint16_t *registers, int addressOffset) {
    std::string readValue;

    for(int i{0}; i<this->registerSize; i++) {
        std::stringstream readValueStringStream;
        readValueStringStream << (registers[addressOffset + this->registerAddressOffset + i] >> 8) << " ";
        readValueStringStream << (registers[addressOffset + this->registerAddressOffset + i] & 0x00FF) << " ";

        readValue.append(readValueStringStream.str().c_str());
    }

    this->value = readValue;
}