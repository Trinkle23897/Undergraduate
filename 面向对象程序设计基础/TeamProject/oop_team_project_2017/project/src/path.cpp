#include "path.h"

bool Path::append(Vertex vertex)
{
	if(size() == 0)
	{
		vertex_.push_back(vertex);
	} else {
		// check parallel
		Vertex tail = vertex_.back();
		if(tail.x != vertex.x && tail.y != vertex.y)
			return false;

		if(size() == 1)
		{
			vertex_.push_back(vertex);
		} else {
			// check whether to merge
			Vertex pre_tail = vertex_[size() - 2];
			if(    (vertex.x == tail.x && tail.x == pre_tail.x)
				|| (vertex.y == tail.y && tail.y == pre_tail.y) )
				vertex_.back() = vertex;
			else vertex_.push_back(vertex);
		}
	}

	return true;
}

bool Path::append(int step)
{
	if(size() < 2) return false;
	Vertex tail = vertex_.back();
	Vertex pre_tail = vertex_[size() - 2];
	if(tail.x == pre_tail.x)
		vertex_.push_back( { tail.x, tail.y + step } );
	else vertex_.push_back( { tail.x + step, tail.y } );
	return true;
}

bool Path::remove()
{
	if(size() == 0) return false;
	vertex_.pop_back();
	return true;
}

int Path::length() const
{
	if(size() < 2) return 0;

	int len = 0;
	VertexSet::const_iterator prev = vertex_.begin();
	VertexSet::const_iterator now = prev + 1;
	VertexSet::const_iterator end = vertex_.end();
	for(; now != end; ++now, ++prev)
	{
		int step = now->x + now->y - (prev->x + prev->y);
		len += step < 0 ? -step : step;
	}

	return len;
}

void Path::print(std::ostream& os) const
{
	for(auto beg = vertex_.begin(), end = vertex_.end(); beg != end; ++beg)
		os << "(" << beg->x << "," << beg->y << (beg + 1 != end ? ")-" : ")");
}

void Path::read(std::istream& is)
{
	vertex_.clear();

	std::string str;
	is >> str;
	for(size_t i = 0; i < str.size(); ++i)
	{
		Vertex v;
		v.x = std::atoi(str.c_str() + ++i);
		while(str[i] != ',') ++i;

		v.y = std::atoi(str.c_str() + ++i);
		while(str[i] != ')') ++i;
		++i;

		append(v);
	}
}
