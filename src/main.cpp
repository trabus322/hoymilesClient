#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "modbus.h"
#include "dtu.h"

int main(){

    std::string ip_address {"192.168.31.136"};
    int port {502};

    auto startTime = std::chrono::high_resolution_clock::now();
    Dtu dtu {ip_address.c_str(), port};
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Construction time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;

    while(true) {
        auto startTime = std::chrono::high_resolution_clock::now();
        dtu.updateMicroinverters();
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "Update time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;
        dtu.printMicroinverters();
    }

    return 0;
}