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
#include "Cost.h"
#include "Dijkstra.h"

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
	
	boost::program_options::variables_map opt;
	bool normal_mode = true; // continue with search after options

	try {
		store(parse_command_line(argc, argv, desc), opt);
		notify(opt); // needed if we ever use a notifier on an option
	}
	catch (const boost::program_options::error &ex) {
		std::cerr << ex.what() << '\n';
		normal_mode = false;
	}

	if(opt.count("help")) {
		std::cout << desc << '\n';
		normal_mode = false;
	}
	
	if(opt.count("graph") + opt.count("departure") + opt.count("destination") != 3) {
		std::cerr << "Mandatory options missing (graph, departure, destination)." << '\n';
		normal_mode = false;
	}

	std::filesystem::path graph_path(opt["graph"].as<std::wstring>());
	std::filesystem::path restrictions_path;
	
	if(opt.count("restrictions"))
		restrictions_path = opt["restrictions"].as<std::wstring>();

	std::filesystem::path report_path;
	
	if(opt.count("report"))
		report_path = opt["report"].as<std::wstring>();

	// early exit
	if(!normal_mode)
	{
		char c;
		std::cin >> c;
		return 0;
	}

	// Read input data
	std::ifstream graph_stream(graph_path);
	std::string graph_text;
	if(!std::getline(graph_stream, graph_text, {})) {
		std::cerr << "Failed to read graph from " << graph_path << "\n";
		char c;
		std::cin >> c;
		return 1;
	}

	AirGraph graph{AirGraph::FromGraphML(graph_text)};
	// print_graph(graph);
	Cost cost;
	Dijkstra dijkstra(graph, cost);
	Counter counter;

	// stopgap solution: dep/dest are identified by their node id
	std::wistringstream dep_stream(opt["departure"].as<std::wstring>());
	std::wistringstream dest_stream(opt["destination"].as<std::wstring>());
	AirGraph::NodeId start, goal;
	dep_stream >> start;
	dest_stream >> goal;

	dijkstra.SetCounter(counter);
	dijkstra.run(start, goal);

	std::ofstream report_fstream;
	std::ostream* report_stream = &std::cout;

	if(!report_path.empty()) {
		report_fstream.open(report_path, std::ios::app);
		if(!report_fstream) {
			std::cerr << "Failed to write report to " << report_path << "\n";
			char c;
			std::cin >> c;
			return 1;
		}
		report_stream = &report_fstream;
	}

	(*report_stream) << counter.ToCsv() << "\n";

	if(report_fstream.is_open())
		report_fstream.close();

	char c;
	std::cin >> c;
	return 0;
}

[[maybe_unused]]
void print_graph(const AirGraph& graph)
{
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
}
