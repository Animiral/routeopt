// RouteSearchRun.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <filesystem>
#include <fstream>
#include <regex>
#include <iostream>
#include <windows.h>
//#include <Processenv.h>
//#include <shellapi.h>
#include <boost/program_options.hpp>
#include "Graph.h"
#include "Cost.h"
#include "Dijkstra.h"
#include "Restriction.h"
#include "Search.h"

namespace
{

/**
 * Convert the given string in Windows' 16-bit Unicode encoding
 * (e.g. from command line or GUI elements) into an UTF-8 string
 * compatible with strings from input files etc.
 */
std::string EncodingSysToU8(std::wstring sys_string);

// debug functions
void print_graph(const AirGraph& graph);
void print_path(Path const& path, AirGraph const& graph);

}


int main()
{
	// convert command line arguments
	LPWSTR command_line = GetCommandLineW();
	int argc;
	LPWSTR* argv = CommandLineToArgvW(command_line, &argc);

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

	// dep/dest are identified by their waypoint name
	std::string departure, destination;
	try {
		departure = EncodingSysToU8(opt["departure"].as<std::wstring>());
		destination = EncodingSysToU8(opt["destination"].as<std::wstring>());
	}
	catch(std::exception ex) {
		std::cerr << ex.what() << "\n";
		char c;
		std::cin >> c;
		return 1;
	}

	// Read input data
	std::string graph_text;

	{
		std::ifstream graph_stream(graph_path);
		if(!std::getline(graph_stream, graph_text, {})) {
			std::cerr << "Failed to read graph from " << graph_path << "\n";
			char c;
			std::cin >> c;
			return 1;
		}
	}

	AirGraph graph{AirGraph::FromGraphML(graph_text)};
	// print_graph(graph);
	AirGraph::Node const* start = graph.FindNodeByName(departure);
	AirGraph::Node const* goal = graph.FindNodeByName(destination);
	if(nullptr == start || nullptr == goal) {
			std::cerr << "Departure/destination not found in the graph: "
				<< departure << "/" << destination << "\n";
			char c;
			std::cin >> c;
			return 1;
	}

	std::vector<Restriction> restrictions;

	if(opt.count("restrictions")) {
		std::ifstream restriction_stream(restrictions_path);
		std::string line, str_a, str_b;

		while(std::getline(restriction_stream, line, '\n')) {
			std::regex token_ex("[^,]+");

			// BUG: this is lacking in error handling
			std::sregex_token_iterator it(line.begin(), line.end(), token_ex), end;
			if(it != end) str_a = *it++;
			if(it != end) str_b = *it++;

			AirGraph::Node const* node_a = graph.FindNodeByName(str_a);
			AirGraph::Node const* node_b = graph.FindNodeByName(str_b);

			if(node_a && node_b) {
				restrictions.push_back({node_a->id, node_b->id});
			}
			else {
				std::cerr << "Restriction points not found in the graph: "
					<< str_a << "/" << str_b << "\n";
				// this is not a fatal error - continue.
			}
		}
	}

	Cost cost;
	Dijkstra dijkstra(graph, cost);
	Counter counter;
	dijkstra.SetCounter(counter);
	Search search{graph, dijkstra, restrictions};
	search.SetCounter(counter);
	Path path = search.Run(start->id, goal->id);

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

	// The actual solution goes to cout for interactive inspection.
	// We do not actually need this information for performance measurement.
	print_path(path, graph);

	char c;
	std::cin >> c;
	return 0;
}

namespace
{

std::string EncodingSysToU8(std::wstring sys_string)
{
	std::string u8string(4 * sys_string.size(), ' ');

	int u8bytes = WideCharToMultiByte(CP_UTF8, 0,
		sys_string.data(), (int)sys_string.size() + 1,
		u8string.data(), (int)u8string.size(), NULL, NULL);
	
	if(0 == u8bytes) {
		throw std::exception("Failed unicode conversion.");
	}

	u8string.resize(u8bytes - 1); // drop 0 terminator as it is handled by std::string
	return u8string;
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

[[maybe_unused]]
void print_path(Path const& path, AirGraph const& graph)
{
	for(int i = 0; i < path.nodes.size(); i++) {
		AirGraph::Node const& node = graph.GetNode(path.nodes[i]);
		std::cout << node.waypoint.name;

		if(i < path.edges.size()) {
			AirGraph::Edge const& edge = graph.GetEdge(path.edges[i]);
			std::cout << " - (" << edge.airway.name << ") - ";
		}
	}

	std::cout << "\nCost: " << path.cost << "\n";
}

}
