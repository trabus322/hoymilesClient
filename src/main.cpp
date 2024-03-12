#include <iostream>
#include <string>

#include "hoymiles.h"
#include "modbus.h"

int main(){

    std::string ip_address {"192.168.31.136"};
    int port {502};

    Dtu dtu {ip_address.c_str(), port};
    dtu.readTest();

    return 0;
}