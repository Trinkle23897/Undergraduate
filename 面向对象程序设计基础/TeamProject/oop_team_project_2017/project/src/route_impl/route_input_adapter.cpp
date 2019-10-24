#include "route_input_adapter.h"
#include <fstream>
#include <algorithm>

RoutePtr get_input_adapter(const char* filename)
{
	return std::make_shared<RouteInputAdapterImpl>(filename);
}

RouteInputAdapterImpl::RouteInputAdapterImpl(const char* filename)
	: filename(filename) {}

void RouteInputAdapterImpl::compute(CallbackFunc)
{
	std::ifstream ifs(filename.c_str());
	if (!ifs) return;

	ifs >> path_;
	grid_x_ = grid_y_ = 0;
	// compute grid size
	for (const Path& p : path_)
	{
		grid_x_ = std::max(grid_x_, p.vertex().back().x);
		grid_y_ = std::max(grid_y_, p.vertex().back().y);
	}
	grid_x_ = grid_y_ = std::max(grid_x_, grid_y_) + 1;
	// compute node size
	if (path_.size() == 1)
	{
		node_x_ = node_y_ = 1, grid_x_ = grid_y_ = 3;
	} else {
		Vertex v1 = path_[0].vertex().front();
		Vertex v2 = path_[1].vertex().front();
		int dist = std::max(v2.x - v1.x, v2.y - v1.y);
		node_x_ = (grid_x_ - 1) / dist - 1;
		node_y_ = (grid_y_ - 1) / dist - 1;
	}
	std::cout << length() << std::endl;
}
