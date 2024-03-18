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

    return 0;
}