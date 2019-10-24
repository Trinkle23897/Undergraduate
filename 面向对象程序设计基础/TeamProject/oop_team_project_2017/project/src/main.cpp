#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>
#include "route.h"
#include "visualization.h"

using namespace boost::program_options;

int main(int argc, char* argv[])
{
	int node_num, win_size, img_size, delay;
	std::string img_filename, algo_type, path_dir, input_dir;

	try
	{
		options_description desc("Options");
		desc.add_options()
		("help,h", "display this help and exit")
		("node-num,n", value<int>(&node_num)->default_value(12), "number of nodes in x direction")
		("algo,a", value<std::string>(&algo_type)->default_value("rule"),
		 "choose which kind of algorithm to solve\navailable: rule, flow, decircle")
		("save-path,p", value<std::string>(&path_dir), "file to save path")
		("input,i", value<std::string>(&input_dir), "file to read path")
		("img-path,o", value<std::string>(&img_filename),
		 "save image to specified file, image won't be saved if this parameter is not specified")
		("img-size", value<int>(&img_size)->default_value(2000), "size of saved image")
		("win-size", value<int>(&win_size)->default_value(800), "size of window")
		("no-window", "do not display the window")
		("show-step", "display the process of computing")
		("delay,d", value<int>(&delay)->default_value(1), "delay between two steps");

		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);

		if (vm.count("help"))
		{
			std::cout << desc << std::endl;
			return 0;
		}

		RoutePtr route;

		if (vm.count("input"))
		{
			route = get_input_adapter(input_dir.c_str());
		} else {
			if (algo_type == "flow")
			{
				route = get_network_flow_algo();
				std::cout << "Using network flow backend to compute." << std::endl;
			} else if(algo_type == "rule") {
				route = get_rule_based_algo();
				std::cout << "Using rule-based backend to compute." << std::endl;
			} else if(algo_type == "decircle") {
				route = get_decircle_algo();
				std::cout << "Using decircle backend to compute." << std::endl;
			} else {
				std::cerr << "Backend " << algo_type << " not recognoized!" << std::endl;
				return -1;
			}

			route->set_node(node_num, node_num);
		}

		auto show_window = [&](int delay)
		{
			char win_name[100];
			std::sprintf(win_name, "Escape Route %dx%d-%dx%d", node_num, node_num, route->grid_x(), route->grid_y());

			std::printf("Paused, cost = %d\n", route->length());

			Visualization visual(win_size, win_size);
			visual.draw_grid(route->grid_x(), route->grid_y());
			visual.draw_route(route);
			visual.show(win_name, delay);
		};

		if (vm.count("show-step"))
		{
			route->compute([&](const Route*) { show_window(delay); } );
		} else route->compute();

		if (vm.count("img-path"))
		{
			Visualization visual(img_size, img_size);
			visual.draw_grid(route->grid_x(), route->grid_y());
			visual.draw_route(route);
			visual.save(img_filename.c_str());
		}

		if (vm.count("save-path"))
		{
			std::ofstream ofs(path_dir.c_str());
			route->print(ofs);
		}

		if (vm.count("no-window") == 0)
			show_window(0);
	} catch (const error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
