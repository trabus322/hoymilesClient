#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include "hoymiles.h"
#include "modbus.h"

int main(){

    std::string ip_address {"192.168.31.136"};
    int port {502};

    // Dtu dtu {ip_address.c_str(), port};
    // while(true) {
    //     dtu.updateMicroinverters();
    // }

    Port hoymilesPort{modbus_new_tcp(ip_address.c_str(), port), 0x1000};

    hoymilesPort.updateParameters();

    std::shared_ptr<PortParameter> temp{hoymilesPort.getParameterById(0)};
    float temp2 = temp.get()->getValue();
    return 0;
}