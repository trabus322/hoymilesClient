#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "CLI11.hpp"
#include "modbus.h"

#include "dtu.h"

void sigHandler(int signal);

int main(int argc, char **argv) {
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);
	signal(SIGABRT, sigHandler);

	std::string version{"v2.0beta"};
	std::cout << version << std::endl;

	CLI::App hoymilesClient{"Client for DTU-Pro/DTU-ProS"};

	std::string ipAddress{"127.0.0.1"};
	std::string ipAddressHelp{"ipv4 address of DTU {default: " + ipAddress + "}"};
	hoymilesClient.add_option<std::string>("-i,--ip_address", ipAddress, ipAddressHelp)->required()->group("Networking");

	int port{502};
	std::string portHelp{"Port of DTU {default: " + std::to_string(port) + "}"};
	hoymilesClient.add_option<int>("-p,--port", port, portHelp)->group("Networking");

	std::vector<std::string> parametersToGet{};
	std::string parametersToGetHelp{"List of parameters to fetch, delimited by ','; possible parameters:\n  - pvVoltage [pvU]\n  - pvCurrentMI [pvIMI]\n  - pvCurrentHM [pvIHM]\n  - gridVoltage [gU]\n  - gridFrequency [gF]\n  - pvPower [gP]\n  - todayProduction [tdP]\n  - totalProduction [ttP]\n  - temperature [t]\n  - operatingStatus [oS]\n  - alarmCode [aC]\n  - alarmCount [aCnt]\n  - linkStatus [lS]"};
	hoymilesClient.add_option<std::vector<std::string>>("-P,--parameters", parametersToGet, parametersToGetHelp)->delimiter(',')->group("Parameters");

	bool allParameters = false;
	std::string allParametersHelp{"Fetch all parameters"};
	hoymilesClient.add_flag<bool>("-a,--all_parameters", allParameters, allParametersHelp)->group("Parameters");

	bool shortNames = false;
	std::string shortNamesHelp{"Print short parameter names"};
	hoymilesClient.add_flag<bool>("-s,--short", shortNames, shortNamesHelp)->group("Parameters");

	std::vector<long long> microinvertersToGet{};
	std::string microinvertersToGetHelp{"List of microinverters to fetch, delimited by ','; if omitted, all are fetched"};
	hoymilesClient.add_option<std::vector<long long>>("-m,--microinverters", microinvertersToGet, microinvertersToGetHelp)->delimiter(',')->group("Microinverters");

	bool microinvertersGetTodayProduction{false};
	std::string microinvertersGetTodayProductionHelp{"Show today production for microinverters"};
	hoymilesClient.add_flag<bool>("-t,--today_production", microinvertersGetTodayProduction, microinvertersGetTodayProductionHelp)->group("Microinverters");

	bool microinvertersGetTotalProduction{false};
	std::string microinvertersGetTotalProductionHelp{"Show total production for microinverters"};
	hoymilesClient.add_flag<bool>("-T,--total_production", microinvertersGetTotalProduction, microinvertersGetTotalProductionHelp)->group("Microinverters");

	bool ignoreNotConnected = false;
	std::string ignoreNotConnectedHelp{"Ignore conn_error"};
	hoymilesClient.add_flag<bool>("-I,--ignore_conn_error", ignoreNotConnected, ignoreNotConnectedHelp)->group("Debug");


	try {
		hoymilesClient.parse(argc, argv);
	} catch (const CLI::ParseError &e) {
		return hoymilesClient.exit(e);
	}

	std::cout << "Mapping out DTU" << std::endl;
	auto startTime = std::chrono::high_resolution_clock::now();
	Dtu dtu{ipAddress.c_str(), port};
	auto endTime = std::chrono::high_resolution_clock::now();
	std::cout << "DTU construction time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;

	while ((dtu.isConnected() || ignoreNotConnected) && (!parametersToGet.empty() || allParameters)) {
		startTime = std::chrono::high_resolution_clock::now();
		dtu.updateMicroinverters(parametersToGet, allParameters, microinvertersToGet);
		endTime = std::chrono::high_resolution_clock::now();
		std::cout << "DTU update time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;

		dtu.printMicroinverters(parametersToGet, allParameters, microinvertersToGet, shortNames, microinvertersGetTodayProduction, microinvertersGetTotalProduction);
		std::cout << std::endl;
	}
	// if(dtu.modbusError()) {
	// 	std::cerr << dtu.modbusErrorMessage() << std::endl;
	// }

	return 0;
}

void sigHandler(int signal) {
	std::cerr << "Interrupted\n";
	exit(0);
}