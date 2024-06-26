#ifndef SUNSPEC_H
#define SUNSPEC_H

#include <string>
#include <stdint.h>
#include <memory>
#include <vector>

#include "modbus.h"
#include "sunspecParametersGeneric.h"

class Sunspec {
      protected:
    modbus_t *modbus;

    std::vector<std::shared_ptr<SunspecParameter>> parameters;

    std::string manufacturer;
    std::string model;
    std::string version;

    std::string serialNumber;
    uint16_t deviceAddress;

    int sunspecAddress;

    void setValues();

    void populateParameters();

      public:
    Sunspec(int address, modbus_t *modbus);

    uint16_t getAddress();
};

#endif