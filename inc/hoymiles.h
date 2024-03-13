#ifndef HOYMILES_H
#define HOYMILES_H

#include <stdint.h>
#include <vector>

struct _modbus;
typedef _modbus modbus_t;

class Dtu{
    private:
    modbus_t *modbus_t;

    std::vector<Microinverter> microinverters;

    public:
    Dtu(const char *ip_address, int port);

    void readTest(uint16_t address, int registers);

    ~Dtu();
};

class Microinverter{
    private:
    uint16_t readArray[33];
    uint16_t address;

    public:
    const int serialNumber;
    std::pair<int, int> portNumber;

    std::pair<float, int> plantVoltage;
    std::pair<float, int> plantCurrent;

    std::pair<float, int> gridVoltage;
    std::pair<float, int> gridFrequency;

    std::pair<float, int> plantPower;

    std::pair<long, int> todayProduction;
    std::pair<long, int> totalProduction;

    std::pair<int, int> temperature;

    std::pair<int, int> operatingStatus;
    std::pair<int, int> alarmCode;
    std::pair<int, int> alarmCount;

    std::pair<int, int> linkStatus;

    Microinverter(uint16_t address);

    void updateValues();
};

#endif