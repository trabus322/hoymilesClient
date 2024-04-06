#include <chrono>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <vector>

#include "CLI11.hpp"
#include "modbus.h"

#include "dtu.h"

void sigHandler(int signal);

int main(int argc, char **argv) {
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);
	signal(SIGABRT, sigHandler);

	std::string version{"v2.2w"};
	std::cout << version << std::endl;

	CLI::App hoymilesClient{"Client for DTU-Pro/DTU-ProS"};

	hoymilesClient.set_version_flag("-v,--version", version);

	std::string ipAddress{"127.0.0.1"};
	std::string ipAddressHelp{"ipv4 address of DTU {default: " + ipAddress + "}"};
	hoymilesClient.add_option<std::string>("-i,--ip_address", ipAddress, ipAddressHelp)->required()->group("Networking");

	int port{502};
	std::string portHelp{"Port of DTU {default: " + std::to_string(port) + "}"};
	hoymilesClient.add_option<int>("-p,--port", port, portHelp)->group("Networking");

	std::vector<std::string> parametersToGet{};
	std::string parametersToGetHelp{"List of parameters to fetch, delimited by ','; possible parameters:\n  - pvVoltage [pvU]\n  - pvCurrentMI [pvIMI]\n  - pvCurrentHM [pvIHM]\n  - gridVoltage [gU]\n  - gridFrequency [gF]\n  - pvPower [gP]\n  - "
					"todayProduction [tdP]\n  - totalProduction [ttP]\n  - temperature [t]\n  - operatingStatus [oS]\n  - alarmCode [aC]\n  - alarmCount [aCnt]\n  - linkStatus [lS]"};
	hoymilesClient.add_option<std::vector<std::string>>("-P,--parameters", parametersToGet, parametersToGetHelp)->delimiter(',')->group("Parameters");

	bool allParameters = false;
	std::string allParametersHelp{"Fetch all parameters"};
	hoymilesClient.add_flag<bool>("-a,--all_parameters", allParameters, allParametersHelp)->group("Parameters");

	bool shortNames = false;
	std::string shortNamesHelp{"Print short parameter names"};
	hoymilesClient.add_flag<bool>("-s,--short", shortNames, shortNamesHelp)->group("Parameters");

	std::vector<long long> microinvertersToChoose{};
	std::string microinvertersToChooseHelp{"List of microinverters to work on, delimited by ','; if omitted, all are selected"};
	hoymilesClient.add_option<std::vector<long long>>("-m,--microinverters", microinvertersToChoose, microinvertersToChooseHelp)->delimiter(',')->group("Microinverters");

	bool microinvertersGetTodayProduction{false};
	std::string microinvertersGetTodayProductionHelp{"Show today production for microinverters"};
	hoymilesClient.add_flag<bool>("-t,--today_production", microinvertersGetTodayProduction, microinvertersGetTodayProductionHelp)->group("Microinverters");

	bool microinvertersGetTotalProduction{false};
	std::string microinvertersGetTotalProductionHelp{"Show total production for microinverters"};
	hoymilesClient.add_flag<bool>("-T,--total_production", microinvertersGetTotalProduction, microinvertersGetTotalProductionHelp)->group("Microinverters");

	bool ignoreNotConnected = false;
	std::string ignoreNotConnectedHelp{"Ignore conn_error"};
	hoymilesClient.add_flag<bool>("-I,--ignore_conn_error", ignoreNotConnected, ignoreNotConnectedHelp)->group("Debug");

	bool writeMode = false;
	std::string writeModeHelp{"Write instead of read"};
	hoymilesClient.add_flag<bool>("-w,--write", writeMode, writeModeHelp)->group("Write");

	uint16_t writeValue;
	std::string writeValueHelp{"Value to write"};
	hoymilesClient.add_option<uint16_t>("-V,--value", writeValue, writeModeHelp)->group("Write")->needs(hoymilesClient.get_option("-w"));

	std::string writeStatusName{};
	std::string writeStatusNameHelp{"Status to write"};
	hoymilesClient.add_option<std::string>("-S,--status", writeStatusName, writeStatusName)->group("Write")->needs(hoymilesClient.get_option("-w"));

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
	std::cout << std::endl << std::endl;

	while (!writeMode && ((dtu.isConnected() || ignoreNotConnected) && (!parametersToGet.empty() || allParameters))) {
		time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		startTime = std::chrono::high_resolution_clock::now();
		dtu.updateMicroinverters(parametersToGet, allParameters, microinvertersToChoose);
		endTime = std::chrono::high_resolution_clock::now();

		std::cout << "Pass time: " << std::put_time(localtime(&now), "%F %T") << std::endl;
		std::cout << "DTU update time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;

		dtu.printMicroinverters(parametersToGet, allParameters, microinvertersToChoose, shortNames, microinvertersGetTodayProduction, microinvertersGetTotalProduction);
		std::cout << std::endl;
	}
	// if(dtu.modbusError()) {
	// 	std::cerr << dtu.modbusErrorMessage() << std::endl;
	// }

	if(writeMode) {
		std::cout << "Starting DTU write" << std::endl;

		startTime = std::chrono::high_resolution_clock::now();
		dtu.setStatusMicroinverters(writeValue, writeStatusName, microinvertersToChoose);
		endTime = std::chrono::high_resolution_clock::now();

		std::cout << "DTU write time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << "ms" << std::endl;
	}

	return 0;
}

void sigHandler(int signal) {
	std::cerr << "Interrupted\n";
	exit(0);
}