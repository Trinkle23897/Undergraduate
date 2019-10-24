#include "route.h"
#include "route_impl/grid_size_constants.h"

int Route::_node2id(int x, int y) const
{
	return x * node_y_ + y;
}

void Route::_id2node(int id, int& x, int& y) const
{
	y = id % node_y_;
	x = id / node_y_;
}

int Route::_grid2id(int x, int y) const
{
	return x * grid_y_ + y;
}

void Route::_id2grid(int id, int& x, int& y) const
{
	y = id % grid_y_;
	x = id / grid_y_;
}

void Route::_node2grid(int nx, int ny, int& gx, int& gy) const
{
	gx = grid_x_ / (node_x_ + 1) * (nx + 1);
	gy = grid_y_ / (node_y_ + 1) * (ny + 1);
}

bool Route::set_node(int node_x, int node_y)
{
	if (node_x <= 0 || node_y <= 0)
		return false;

	path_.clear();
	if (node_x < node_y)
	{
		int tmp = node_x;
		node_x = node_y;
		node_y = tmp;
	}
	node_x_ = node_x;
	node_y_ = node_y;

	return true;
}

int Route::length() const
{
	int len = 0;
	for (const Path& p : path_)
		len += p.length();
	return len;
}

void Route::print(std::ostream& os) const
{
	os << path_;
}

void Route::compute_grid_size(int &gx, int &gy)
{
	if (node_x_ <= route_grid_size_maximum && node_y_ <= route_grid_size_maximum)
	{
		gx = gy = (node_y() + 1) * route_grid_size[node_x_] + 1;
	} else gx = gy = -1;
}

bool Route::_validate() const
{
	typedef std::vector<int> IntList;
	typedef std::vector<IntList> VecList;
	VecList mark(grid_x() + 1, IntList(grid_y() + 1, 0));
	int flag = 1;
	for (const Path& p : path_)
	{
		if (p.vertex().size() <= 1)
		{
			printf("[%d,%d] : no path\n", (p.vertex().begin())->x, (p.vertex().begin())->y);
			flag = 0;
		}
		auto prev = p.vertex().begin(),
		     now  = prev + 1,
		     end  = p.vertex().end();
		auto begin_ = prev;

		for (; now != end; ++now, ++prev)
		{
			int dx = 0, dy = 0;

			if (prev->x != now->x)
				dx = prev->x < now->x ? 1 : -1;
			else dy = prev->y < now->y ? 1 : -1;

			int x = prev->x, y = prev->y;

			while (x != now->x || y != now->y)
			{
				if (mark[x][y])
				{
					printf("[%d,%d] path illegal\n", begin_->x, begin_->y);
					flag = 0;
					goto loop;
				}

				mark[x][y] = 1;
				x += dx, y += dy;
			}
		}
loop:
		;
	}

	return flag;
}
