#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "CLI11.hpp"
#include "modbus.h"

#include "dtu.h"

int main(int argc, char **argv) {

	CLI::App hoymilesClient{"Client for DTU-Pro/DTU-ProS"};

	std::string ipAddress{"127.0.0.1"};
	std::string ipAddressHelp{"ipv4 address of DTU {default: " + ipAddress + "}"};
	hoymilesClient.add_option<std::string>("-i,--ip_address", ipAddress, ipAddressHelp);

	int port{502};
	std::string portHelp{"Port of DTU {default: " + std::to_string(port) + "}"};
	hoymilesClient.add_option<int>("-p,--port", port, portHelp);

	std::vector<std::string> parametersToGet{};
	std::string parametersToGetHelp{"List of parameters to fetch, delimited by ',', example[par1,par2,par3]"};
	hoymilesClient.add_option<std::vector<std::string>>("-P,--parameters", parametersToGet, parametersToGetHelp)->delimiter(',');

	bool allParameters = false;
	std::string allParametersHelp{"Fetch all parameters"};
	hoymilesClient.add_flag<bool>("-a,--all_parameters", allParameters, allParametersHelp);

	bool ignoreNotConnected = false;
	std::string ignoreNotConnectedHelp{"Ignore connection errors"};
	hoymilesClient.add_flag<bool>("-I,--ignore_conn_error", ignoreNotConnected, ignoreNotConnectedHelp);

	auto startTime = std::chrono::high_resolution_clock::now();
	Dtu dtu{ipAddress.c_str(), port};
	auto endTime = std::chrono::high_resolution_clock::now();
	std::cout << "DTU construction time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;

	while (dtu.isConnected() || ignoreNotConnected) {
		if (allParameters) {
			startTime = std::chrono::high_resolution_clock::now();
			dtu.updateMicroinverters();
			endTime = std::chrono::high_resolution_clock::now();
			std::cout << "DTU update time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;

			dtu.printMicroinverters();
		} else {
			startTime = std::chrono::high_resolution_clock::now();
			dtu.updateMicroinverters(parametersToGet);
			endTime = std::chrono::high_resolution_clock::now();
			std::cout << "DTU update time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;

			dtu.printMicroinverters(parametersToGet);
		}
	}

	return 0;
}