#include <iostream>
#include <string>

#include <chrono>
#include <thread>

#include "hoymiles.h"
#include "modbus.h"

int main(){

    std::string ip_address {"192.168.31.136"};
    int port {502};

    Dtu dtu {ip_address.c_str(), port};

    bool buttonPressed{false};
   for(int i{0}; i<10; i++){
        dtu.readTest(0x1034, 2);
        std::clog << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}