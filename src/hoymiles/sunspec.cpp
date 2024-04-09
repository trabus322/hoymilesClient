#include <string>
#include <stdint.h>
#include <memory>

#include "sunspec.h"
#include "modbus.h"
#include "sunspecParameters.h"

Sunspec::Sunspec(int address, modbus_t *modbus) {
    this->modbus = modbus;
    this->sunspecAddress = address;

    this->populateParameters();

    this->setValues();
}

void Sunspec::setValues() {
    uint16_t registers[70];

    int registerCount;
    registerCount = modbus_read_registers(this->modbus, this->sunspecAddress, 70, registers);

    std::vector<std::shared_ptr<SunspecParameter>>::iterator parametersIterator = this->parameters.begin();
    while(parametersIterator != this->parameters.end()) {
        parametersIterator->get()->getValueFromRegisters(registers, 0);
        parametersIterator++;
    }
}

void Sunspec::populateParameters() {
    SunspecParameterManufacturer manufacturer{};
    this->parameters.push_back(std::make_shared<SunspecParameterManufacturer>(manufacturer));
}