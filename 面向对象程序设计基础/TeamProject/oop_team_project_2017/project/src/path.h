#ifndef __PATH_H__
#define __PATH_H__

#include <vector>
#include <iostream>
#include "vertex.h"

class Path
{
	VertexSet vertex_;
public:
	int size() const { return vertex_.size(); }
	const VertexSet& vertex() const { return vertex_; }

	/* add a vertex to the tail of the path. If the latest two vertexs and the
	 * new one are in the same line, they will be merged into two vertexs
	 * return true if the vertex is successfully added */
	bool append(Vertex vertex);

	/* add a vertex goes forwards `step` steps
	 * only available when the path has more than two vertexs so that the
	 * direction can be determinted */
	bool append(int step);

	/* remove last vertex */
	bool remove();

	/* compute path length */
	int length() const;

	void print(std::ostream& os = std::cout) const;
	void read(std::istream& is = std::cin);
};

typedef std::vector<Path> PathSet;

inline std::ostream& operator << (std::ostream& os, const Path& p)
{
	p.print(os);
	return os;
}

inline std::istream& operator >> (std::istream& is, Path& p)
{
	p.read(is);
	return is;
}

inline std::ostream& operator << (std::ostream& os, const PathSet& ps)
{
	for(const Path& p : ps)
		os << p << "\n";
	return os;
}

inline std::istream& operator >> (std::istream& is, PathSet& ps)
{
	Path p;
	while(is >> p)
		ps.push_back(p);
	return is;
}

#endif
