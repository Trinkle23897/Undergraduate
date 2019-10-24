#ifndef __VISUALIZATION_H__
#define __VISUALIZATION_H__

#include "route.h"
#include <opencv2/core.hpp>

class Visualization
{
	int w_, h_;
	cv::Mat img_;
public:
	Visualization(int width, int height);

	void draw_grid(int grid_x, int grid_y);
	void draw_route(const RoutePtr route);
	void show(const char* window_name, int delay = 0);
	void save(const char* filename);
};

#endif
