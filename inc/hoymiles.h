#ifndef HOYMILES_H
#define HOYMILES_H

#include <stdint.h>
#include <vector>
#include <string>

struct _modbus;
typedef _modbus modbus_t;

class MicroinverterParameter {
    public:
    std::string name;

    int valueInt;
    float valueFloat;

    int age;

    uint16_t addressOffset;
    int registerSize;

    MicroinverterParameter(std::string name, uint16_t addressOffset, int registerSize);

    void updateValue(modbus_t *modbus_context, uint16_t microinverterAddress); 
};

class Microinverter{
    private:
    modbus_t *modbus_context;
    uint16_t address;

    std::vector<MicroinverterParameter> parameters;

    public:
    Microinverter(modbus_t *modbus_t, uint16_t address);

    void updateParameters();

    void updateParameterByIndex(int i);

    void updateParameterByName(std::string name);

    MicroinverterParameter getParameterByIndex(int i);

    MicroinverterParameter getParameterByName(std::string name);
};

class Dtu{
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