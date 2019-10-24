#include "visualization.h"
#include <algorithm>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

Visualization::Visualization(int w, int h)
	: w_(w), h_(h), img_(h, w, CV_8UC3, cv::Scalar(255, 255, 255))
{
}

void Visualization::draw_grid(int grid_x, int grid_y)
{
	cv::Scalar color(200, 200, 200);
	double step_x = 1.0 * w_ / grid_x;
	double step_y = 1.0 * h_ / grid_y;
	cv::Point offset(0.5 * step_x, 0.5 * step_y);

	double x = 0, y = 0;

	for (int i = 0; i < grid_x; ++i, x += step_x)
		cv::line(img_, cv::Point(x, 0) + offset, cv::Point(x, h_ - step_y) + offset, color);

	for (int i = 0; i < grid_y; ++i, y += step_y)
		cv::line(img_, cv::Point(0, y) + offset, cv::Point(w_ - step_x, y) + offset, color);
}

void Visualization::draw_route(const RoutePtr route)
{
	double scale_x = 1.0 * w_ / route->grid_x();
	double scale_y = 1.0 * h_ / route->grid_y();
	cv::Point offset(0.5 * scale_x, 0.5 * scale_y);

	int line_width = scale_x < 3.0 ? 1 : 2;

	// draw path
	cv::Scalar path_color(0, 200, 0);
	for (int i = 0; i != route->node_x(); ++i)
	{
		for (int j = 0; j != route->node_y(); ++j)
		{
			const VertexSet& path = route->path(i, j).vertex();
			//couldn't find path
			if (path.size() <= 1)continue;
			auto prev = path.begin(), now = prev + 1, end = path.end();
			for (; now != end; ++now, ++prev)
			{
				cv::Point p1(prev->x * scale_x, prev->y * scale_y);
				cv::Point p2(now->x * scale_x,  now->y * scale_y);
				cv::line(img_, p1 + offset, p2 + offset, path_color, line_width);
			}
		}
	}

	auto normalize = [=](cv::Point p)
	{
		if(p.x < 0) p.x = 0;
		if(p.x > w_) p.x = w_;
		if(p.y < 0) p.y = 0;
		if(p.y > h_) p.y = h_;
		return p;
	};

	// draw internal nodes
	double rect_scale = 0.2;
	cv::Scalar node_color(200, 0, 0);
	for (int i = 0; i != route->node_x(); ++i)
	{
		for (int j = 0; j != route->node_y(); ++j)
		{
			double cr_scale = 0.0;

			if(route->path(i, j).size() >= 2)
			{
				// calc distance between end point to center
				Vertex end_p = route->path(i, j).vertex().back();
				if (end_p.x == 0 || end_p.x == route->grid_y() - 1)
					cr_scale = (2.0 * end_p.y - route->grid_y()) / route->grid_y();
				else cr_scale = (2.0 * end_p.x - route->grid_x()) / route->grid_x();

				if (cr_scale < 0) cr_scale = -cr_scale;
				if (cr_scale > 1.0) cr_scale = 1.0;
			}

			cv::Scalar color_delta(0, 250 * cr_scale, 250 * cr_scale);

			int gx, gy;
			route->node2grid(i, j, gx, gy);
			cv::Point p1((gx - rect_scale) * scale_x, (gy - rect_scale) * scale_y);
			cv::Point p2((gx + rect_scale) * scale_x, (gy + rect_scale) * scale_y);

			cv::rectangle(
				img_,
				normalize(p1 + offset),
				normalize(p2 + offset),
				node_color + color_delta,
				CV_FILLED
			);
		}
	}

	// draw boundary nodes
	auto draw_circle = [ = ](int x, int y) {
		double r = 0.2 * std::min(scale_x, scale_y);
		cv::Point center(x * scale_x, y * scale_y);
		cv::circle(img_, normalize(center + offset), r, node_color, CV_FILLED);
	};

	for (int i = 1; i < route->grid_x() - 1; ++i)
	{
		draw_circle(i, 0);
		draw_circle(i, route->grid_y() - 1);
	}

	for (int i = 1; i < route->grid_y() - 1; ++i)
	{
		draw_circle(0, i);
		draw_circle(route->grid_x() - 1, i);
	}
}

void Visualization::show(const char* window_name, int delay)
{
	cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	cv::imshow(window_name, img_);
	cv::waitKey(delay);
}

void Visualization::save(const char* filename)
{
	cv::imwrite(filename, img_);
}
