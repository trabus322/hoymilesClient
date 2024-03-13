#ifndef HOYMILES_H
#define HOYMILES_H

#include <stdint.h>
#include <vector>

struct _modbus;
typedef _modbus modbus_t;

class Microinverter{
    private:
    modbus_t *modbus_context;
    uint16_t address;

    void updatePortNumber();

    void updatePlantVoltage();
    void updatePlantCurrent();

    void updateGridVoltage();
    void updateGridFrequency();

    void updatePlantPower();

    void updateTodayProduction();
    void updateTotalProduction();

    void updateTemperature();

    void updateOperatingStatus();
    void updateAlarmCode();
    void updateAlarmCount();

    void updateLinkStatus();

    public:
    int serialNumber;
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

    Microinverter(modbus_t *modbus_t, uint16_t address);

    void updateValues();
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