#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "path.h"
#include <iostream>
#include <memory>
#include <functional>

class Route;
typedef std::shared_ptr<Route> RoutePtr;
typedef std::function<void(const Route*)> CallbackFunc;

class Route
{
protected:
	// number of nodes
	int node_x_, node_y_;
	// size of grids
	int grid_x_, grid_y_;
	PathSet path_;
public:
	virtual ~Route() = default;

	const Path& path(int x, int y) const { return path_[_node2id(x, y)]; }
	int node_x() const { return node_x_; }
	int node_y() const { return node_y_; }
	int grid_x() const { return grid_x_; }
	int grid_y() const { return grid_y_; }

	// map node to its coordinate
	void node2grid(int nx, int ny, int& gx, int& gy) const { _node2grid(nx, ny, gx, gy); }
	Vertex node2grid(Vertex S)const { Vertex T; _node2grid(S.x, S.y, T.x, T.y); return T; }
	// compute total length of paths
	int length() const;

	bool set_node(int node_x, int node_y);

public:
	/* compute routing method */
	virtual void compute(CallbackFunc = nullptr) = 0;
	/* compute grid size, -1 if failed */
	virtual void compute_grid_size(int &gx, int &gy);
public:
	// print path info
	void print(std::ostream& os = std::cout) const;

protected:
	// map node to index (both of them start from 0)
	int _node2id(int x, int y) const;
	// map index to node
	void _id2node(int id, int& x, int& y) const;
	// map node to its coordinate
	void _node2grid(int nx, int ny, int& gx, int& gy) const;
	// map grid to index (both of them start from 0)
	int _grid2id(int x, int y) const;
	// map index to grid
	void _id2grid(int id, int& x, int& y) const;

	// check whether the answer is dis-joint
	bool _validate() const;
};

RoutePtr get_network_flow_algo();
RoutePtr get_rule_based_algo();
RoutePtr get_decircle_algo();
RoutePtr get_input_adapter(const char* filename);

#endif
