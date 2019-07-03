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
	const auto nodes = graph.GetNodes();
	for(const auto node : nodes) {
		Waypoint waypoint = node.waypoint;
		std::cout << "Waypoint: lat " << waypoint.latitude
			<< "  lng " << waypoint.longitude
			<< "  fl " << waypoint.flightlevel
			<< "\n";
		for(AirGraph::EdgeId out_edge : node.out_edges) {
			AirGraph::Edge edge = graph.GetEdge(out_edge);
			Airway airway = edge.airway;
			Waypoint to = graph.GetNode(edge.to).waypoint;
			std::cout << "\t-> (gm " << airway.grossmass
				<< "  dist " << airway.distance
				<< "  time " << airway.time
				<< "  fuel " << airway.fuel
				<< ") [" << to.latitude << "," << to.longitude << "]\n";
		}
	}

	char c;
	std::cin >> c;

    return 0;
}

