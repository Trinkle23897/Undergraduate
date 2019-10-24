#include "route_network_flow.h"
#include <cmath>
#include <iostream>
#include <set>
#include <queue>
#include <algorithm>

RoutePtr get_network_flow_algo()
{
	return std::make_shared<RouteNetworkFlowImpl>(false);
}

RoutePtr get_decircle_algo()
{
	return std::make_shared<RouteNetworkFlowImpl>(true);
}

int RouteNetworkFlowImpl::grid_id(int x, int y) const
{
	return _grid2id(x, y) * 2 + 1;
}

void RouteNetworkFlowImpl::grid_xy(int id, int& x, int& y) const
{
	_id2grid((id - 1) >> 1, x, y);
}

void RouteNetworkFlowImpl::build_model(int& S, int& T)
{
	int grid_num = grid_x() * grid_y();
	S = grid_num * 2 + 2, T = S + 1;

	init_graph(T);

	const int delta_x[] = { 0, 0, 1, -1 };
	const int delta_y[] = { 1, -1, 0, 0 };

	for (int i = 0; i != grid_x(); ++i)
	{
		for (int j = 0; j != grid_y(); ++j)
		{
			int gid_in = grid_id(i, j), gid_out = gid_in + 1;
			// constraint for grid
			add_edge(gid_in, gid_out, 1, 0);

			for (int k = 0; k != 4; ++k)
			{
				int dx = i + delta_x[k], dy = j + delta_y[k];
				if (dx < 0 || dy < 0 || dx >= grid_x() || dy >= grid_y())
					continue;

				add_edge(gid_out, grid_id(dx, dy), 1, 1);
			}
		}
	}

	// constraint for internal nodes
	for (int i = 0; i != node_x(); ++i)
	{
		for (int j = 0; j != node_y(); ++j)
		{
			int gx, gy;
			_node2grid(i, j, gx, gy);
			add_edge(S, grid_id(gx, gy), 1, 0);
		}
	}

	// constraint for boundary nodes
	for (int i = 1; i < grid_x() - 1; ++i)
	{
		add_edge(grid_id(i, 0) + 1, T, 1, 0);
		add_edge(grid_id(i, grid_y() - 1) + 1, T, 1, 0);
	}

	for (int i = 1; i < grid_y() - 1; ++i)
	{
		add_edge(grid_id(0, i) + 1, T, 1, 0);
		add_edge(grid_id(grid_x() - 1, i) + 1, T, 1, 0);
	}
}

Path RouteNetworkFlowImpl::compute_path(int gx, int gy, int T)
{
	Path path;
	std::set<int> M;
	int u = grid_id(gx, gy);

	while (u != T)
	{
		++u; // in -> out

		int x, y;
		grid_xy(u, x, y);

		if(M.count(u) > 0)
			return {};
		else M.insert(u);

		path.append( { x, y } );

		bool found = false;

		for (int k = head_[u]; k; k = next_[k])
		{
			if (e_[k].cost >= 0 && !e_[k].cap)
			{
				u = e_[k].point;
				found = true;
				break;
			}
		}

		if(!found) return {};
	}

	return path;
}

void RouteNetworkFlowImpl::compute_grid_size(int &gx, int &gy)
{
	// call the base class function, check whether size can be computed
	Route::compute_grid_size(gx, gy);

	if (gx == -1)
	{
		// save old variables
		int old_grid_x = grid_x_, old_grid_y = grid_y_;

		// binary search for the minimum grid size
		int L = 1, R = std::max(node_x(), node_y()) + 1;
		while (L < R)
		{
			int M = (L + R) >> 1;

			grid_x_ = (node_x() + 1) * M + 1;
			grid_y_ = (node_y() + 1) * M + 1;

			std::printf("Checking %dx%d...", grid_x_, grid_y_);
			std::fflush(stdout);

			int S, T;
			build_model(S, T);

			int flow = compute_maximum_flow(S, T);

			std::printf("\rChecked %dx%d, flow = %d\n", grid_x_, grid_y_, flow);

			if (flow == node_x() * node_y())
				R = M;
			else L = M + 1;
		}

		// restore old variables
		grid_x_ = old_grid_x, grid_y_ = old_grid_y;

		// compute solution
		gx = (node_x() + 1) * R + 1;
		gy = (node_y() + 1) * R + 1;
	}

	std::printf("Found grid size %dx%d.\n", gx, gy);
}

void RouteNetworkFlowImpl::compute(CallbackFunc callback)
{
	compute_grid_size(grid_x_, grid_y_);

	int S, T;
	build_model(S, T);

	std::printf("Computing solution...");
	if(use_decircle_)
	{
		compute_maximum_flow(S, T, callback);
		while(decircle(S, T))
			do_callback(T, callback);
	} else {
		int cost, flow;
		compute_costflow(S, T, flow, cost, callback);
	}

	path_.clear();
	for (int i = 0; i != node_x(); ++i)
	{
		for (int j = 0; j != node_y(); ++j)
		{
			int gx, gy;
			_node2grid(i, j, gx, gy);
			path_.push_back(compute_path(gx, gy, T));
		}
	}

	std::printf("\rFinished, cost = %d\n", length());

	if (!_validate())
		std::cerr << "ERROR!" << std::endl;
}

void RouteNetworkFlowImpl::init_graph(int node_num)
{
	e_.clear(); next_.clear();

	// placeholder to make index of edge starts from 2
	e_.push_back(Edge()); next_.push_back(0);
	e_.push_back(Edge()); next_.push_back(0);

	head_.assign(node_num + 1, 0);
}

void RouteNetworkFlowImpl::add_edge(int u, int v, int cap, int cost)
{
	next_.push_back(head_[u]);
	head_[u] = next_.size() - 1;
	e_.push_back(Edge(u, v, cap, cost));

	next_.push_back(head_[v]);
	head_[v] = next_.size() - 1;
	e_.push_back(Edge(v, u, 0, -cost));
}

int RouteNetworkFlowImpl::shortest_path(
    int s, int t, IntList& dist, IntList& prev, bool check_circle)
{
	int node_num = head_.size();
	dist.assign(node_num, node_num);
	prev.resize(node_num);

	IntList enque_num(node_num, 0);
	IntList in_que(node_num, 0);
	std::queue<int> Q;

	in_que[s] = 1;
	dist[s] = 0;
	++enque_num[s];
	Q.push(s);

	while (!Q.empty())
	{
		int u = Q.front();
		for (int k = head_[u]; k; k = next_[k])
		{
			const Edge& e = e_[k];
			int t = dist[u] + e.cost;
			if (e.cap && t < dist[e.point])
			{
				dist[e.point] = t;
				prev[e.point] = k;
				if (!in_que[e.point])
				{
					in_que[e.point] = 1;
					if(check_circle && ++enque_num[e.point] > std::sqrt(node_num))
						return e.point;
					Q.push(e.point);
				}
			}
		}

		Q.pop();
		in_que[u] = 0;
	}

	if(check_circle) return 0;
	return dist[t] < node_num;
}

void RouteNetworkFlowImpl::compute_costflow(
	int s,
	int t,
	int& flow,
	int& cost,
	CallbackFunc callback)
{
	cost = 0, flow = 0;
	IntList dist, prev;

	while (shortest_path(s, t, dist, prev))
	{
		int cnt = 0, min_cap = ~0u >> 1;
		for (int u = t; u != s; u = e_[prev[u]].from)
			min_cap = std::min(min_cap, e_[prev[u]].cap);
		for (int u = t; u != s; u = e_[prev[u]].from)
		{
			int k = prev[u];
			e_[k].cap -= min_cap;
			e_[k ^ 1].cap += min_cap;
			cnt += e_[k].cost;
		}

		flow += min_cap;
		cost += cnt * min_cap;

		do_callback(t, callback);
	}
}

bool RouteNetworkFlowImpl::dinic_labeling(int s, int t)
{
	label_.assign(head_.size(), 0);
	std::queue<int> Q;
	label_[t] = 1;
	Q.push(t);
	while (!Q.empty())
	{
		int u = Q.front();
		Q.pop();

		for (int k = head_[u]; k; k = next_[k])
		{
			int v = e_[k].point;
			if (e_[k ^ 1].cap && !label_[v])
			{
				label_[v] = label_[u] + 1;
				Q.push(v);
				if (v == s) return true;
			}
		}
	}

	return label_[s] != 0;
}

int RouteNetworkFlowImpl::dinic_augment(int t, int u, int flow_limit)
{
	if (u == t) return flow_limit;

	int flow_used = 0;
	for (int& k = cur_[u]; k; k = next_[k])
	{
		int v = e_[k].point;
		if (!e_[k].cap || label_[v] + 1 != label_[u])
			continue;
		int flow_avail = std::min(flow_limit - flow_used, e_[k].cap);
		int flow = dinic_augment(t, v, flow_avail);
		flow_used += flow;
		e_[k].cap -= flow;
		e_[k ^ 1].cap += flow;
		if (flow_used == flow_limit)
			return flow_used;
	}

	return flow_used;
}

int RouteNetworkFlowImpl::compute_maximum_flow(int s, int t, CallbackFunc callback)
{
	int flow = 0;
	while (dinic_labeling(s, t))
	{
		cur_ = head_;
		flow += dinic_augment(t, s, ~0u >> 1);
		do_callback(t, callback);
	}

	return flow;
}

int RouteNetworkFlowImpl::decircle(int s, int t)
{
	IntList dist, prev;
	int u = shortest_path(t, s, dist, prev, true);
	if(!u) return 0;

	IntList& mark = dist;  // reuse dist
	std::fill(mark.begin(), mark.end(), 0);

	// find the first node which must be in the circle
	for(; !mark[u]; u = e_[prev[u]].from)
		mark[u] = 1;

	int cost = 0, k = prev[u];
	do {
		--e_[k].cap;
		++e_[k ^ 1].cap;
		cost += e_[k].cost;
		k = prev[e_[k].from];
	} while(k != prev[u]);

	return cost;
}

void RouteNetworkFlowImpl::do_callback(int T, CallbackFunc callback)
{
	if(callback)
	{
		path_.clear();
		for (int i = 0; i != node_x(); ++i)
		{
			for (int j = 0; j != node_y(); ++j)
			{
				int gx, gy;
				_node2grid(i, j, gx, gy);
				path_.push_back(compute_path(gx, gy, T));
			}
		}
		callback(this);
	}
}
