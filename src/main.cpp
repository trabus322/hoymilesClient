#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include "modbus.h"
#include "dtu.h"

int main(){

    std::string ip_address {"192.168.31.136"};
    int port {502};

    Dtu dtu {ip_address.c_str(), port};
    while(true) {
        dtu.updateMicroinverters();
    }

    // Test commit

    // Port hoymilesPort{modbus_new_tcp(ip_address.c_str(), port), 0x1000};

    // hoymilesPort.updateParameters();

    return 0;
}