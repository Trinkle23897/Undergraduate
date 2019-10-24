#ifndef __ROUTE_NETWORK_FLOW_H__
#define __ROUTE_NETWORK_FLOW_H__

#include "../route.h"
#include <vector>

class RouteNetworkFlowImpl : public Route
{
	struct Edge
	{
		int point, from;
		int cap, cost;

		Edge(int u = 0, int v = 0, int cap = 0, int cost = 0)
			: point(v), from(u), cap(cap), cost(cost) {}
	};

	typedef std::vector<int> IntList;
	typedef std::vector<Edge> EdgeList;

	// simulate list using array
	IntList head_, next_;
	EdgeList e_;

	// dinic array
	IntList cur_, label_;

	// which algo to use
	bool use_decircle_;
private:
	void init_graph(int node_num);
	void add_edge(int u, int v, int cap, int cost);

	// cost flow
	int shortest_path(int s, int t, IntList& dist, IntList& prev, bool check_circle = false);
	void compute_costflow(int s, int t, int& flow, int& cost, CallbackFunc);

	// maximum flow
	bool dinic_labeling(int s, int t);
	int dinic_augment(int t, int u, int flow_limit);
	int compute_maximum_flow(int s, int t, CallbackFunc = nullptr);
	int decircle(int s, int t);

	void build_model(int& S, int& T);
	Path compute_path(int gx, int gy, int T);

	void do_callback(int T, CallbackFunc);

	// map grid (x, y) to its index
	int grid_id(int x, int y) const;
	// map grid index to its coordinate
	void grid_xy(int id, int& x, int& y) const;
public:
	void compute(CallbackFunc);
	void compute_grid_size(int& gx, int& gy);

	RouteNetworkFlowImpl(bool use_decircle) : use_decircle_(use_decircle) {}
};

#endif
