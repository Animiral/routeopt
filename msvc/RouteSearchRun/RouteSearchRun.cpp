// RouteSearchRun.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <windows.h>
//#include <Processenv.h>
//#include <shellapi.h>
#include "Graph.h"

int main()
{
	LPWSTR commandLine = GetCommandLineW();
	int argc;
	LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);
	std::filesystem::path file(argv[1]);
	
	std::ifstream ifstream(file);
	std::string graphText;
	if(!std::getline(ifstream, graphText, {})) {
		std::cerr << "Failed to read file " << file << "\n";
		char c;
		std::cin >> c;
		return 1;
	}

	AirGraph graph{AirGraph::FromGraphML(graphText)};
	const auto waypoints = graph.Waypoints();
	for(const auto waypoint : waypoints) {
		std::cout << "Waypoint: lat " << waypoint.latitude
			<< "  lng " << waypoint.longitude
			<< "  fl " << waypoint.flightlevel
			<< "\n";
		const auto& airways = graph.Outgoing(waypoint);
		for(auto& airway : airways) {
			std::cout << "\t-> (gm " << airway.grossmass
				<< "  dist " << airway.distance
				<< "  time " << airway.time
				<< "  fuel " << airway.fuel
				<< ") [" << airway.to.latitude << "," << airway.to.longitude << "]\n";
		}
	}

	char c;
	std::cin >> c;

    return 0;
}

