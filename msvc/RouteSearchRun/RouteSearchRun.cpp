// RouteSearchRun.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <windows.h>
//#include <Processenv.h>
//#include <shellapi.h>
#include <boost/program_options.hpp>
#include "Graph.h"

int main()
{
	// convert command line arguments
	LPWSTR commandLine = GetCommandLineW();
	int argc;
	LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);

	// parse command-line options
	boost::program_options::options_description desc{"Options"};
	using boost::program_options::wvalue;

	desc.add_options()
		("help,h", "Display Usage")
		("graph,g", wvalue<std::wstring>(), "Graph File")
		("restrictions,r", wvalue<std::wstring>(), "Restrictions File")
		("departure,dep", wvalue<std::wstring>(), "Departure Waypoint Identifier")
		("destination,dest", wvalue<std::wstring>(), "Destination Waypoint Identifier")
		("report,o", wvalue<std::wstring>(), "Performance Report Output File");
		//("graph", value<std::string>()->default_value(3.14f), "Pi")
		//("age", value<int>()->notifier(on_age), "Age");
	
	boost::program_options::variables_map vm;
	bool normal_mode = true; // continue with search after options

	try {
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm); // needed if we ever use a notifier on an option
	}
	catch (const boost::program_options::error &ex) {
		std::cerr << ex.what() << '\n';
		normal_mode = false;
	}

	if(vm.count("help")) {
		std::cout << desc << '\n';
		normal_mode = false;
	}
	
	if(vm.count("graph") + vm.count("departure") + vm.count("destination") != 3) {
		std::cerr << "Mandatory options missing (graph, departure, destination)." << '\n';
		normal_mode = false;
	}

	std::filesystem::path graph_path(vm["graph"].as<std::wstring>());

	if(!std::filesystem::is_regular_file(graph_path)) {
		std::cerr << "Not a regular file: " << graph_path;
		normal_mode = false;
	}

	// early exit
	if(!normal_mode)
	{
		char c;
		std::cin >> c;
		return 0;
	}

	// (vm.count("restrictions"))
	// (vm.count("departure"))
	// (vm.count("destination"))
	// (vm.count("report"))

	// Read input data
	std::ifstream ifstream(graph_path);
	std::string graphText;
	if(!std::getline(ifstream, graphText, {})) {
		std::cerr << "Failed to read graph from " << graph_path << "\n";
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

