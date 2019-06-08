#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include "GPTree.h"

const int p2[2][2] = {{0, 1}, {1, 0}};
const int p3[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
const int p4[24][4] = {
    {0, 1, 2, 3}, {0, 1, 3, 2}, {0, 2, 1, 3}, {0, 2, 3, 1}, {0, 3, 1, 2}, {0, 3, 2, 1},
    {1, 0, 2, 3}, {1, 0, 3, 2}, {1, 2, 0, 3}, {1, 2, 3, 0}, {1, 3, 0, 2}, {1, 3, 2, 0},
    {2, 0, 1, 3}, {2, 0, 3, 1}, {2, 1, 0, 3}, {2, 1, 3, 0}, {2, 3, 0, 1}, {2, 3, 1, 0},
    {3, 0, 1, 2}, {3, 0, 2, 1}, {3, 1, 0, 2}, {3, 1, 2, 0}, {3, 2, 0, 1}, {3, 2, 1, 0}
};

// coordinates
struct Coords {
	double lng, lat;
	int id;
	Coords() { lng = lat = id = 0; }
};

typedef Coords Passenger;

struct Car {
	int id, np;
	Coords pos;
	std::vector<Passenger> pass;
	int order[10]; // routine: 0 -> 1 -> 2 -> ...
	int dist;      // distance to current position
	int detour0;   // D2 + D3 - D1
	int detour1;   // D3 - D4
	Car(): pass(0) { id = np = 0; }
};

// car and it's cost
struct CarItem {
	int id;
	double cost;
	bool operator < (const CarItem& a) const { return cost < a.cost; }
};

// number of cars
int nCars = 0;
std::vector<Car> cars;

// available cars
std::vector<int> all_cars;     // id
std::vector<int> all_cars_pos; // pos_id

std::vector<Coords> coords;

void read_node() {
	FILE *f = fopen("../data/road.cnode", "r");
	int n = gpt_getsize();
	coords.resize(n);
	for (int i = 0; i < n; ++i)
		fscanf(f, "%d%lf%lf", &coords[i].id, &coords[i].lng, &coords[i].lat);
	fclose(f);
}

void read_car() {
	FILE *f = fopen("../data/car.txt", "r");
	int id, np;
	while (fscanf(f, "%d%d", &id, &np) == 2) {
		Car car;
		Coords coord;
		car.id = id;
		car.np = np;
		fscanf(f, "%lf,%lf,%d", &coord.lng, &coord.lat, &coord.id);
		car.pos = coord;
		for (int _ = 0; _ < np; ++_) {
			fscanf(f, "%lf,%lf,%d", &coord.lng, &coord.lat, &coord.id);
			car.pass.emplace_back(coord);
		}
		cars.push_back(car);
	}
	nCars = cars.size();
	fclose(f);
}

#define dis(a, b, c, d) (((a) - (c)) * ((a) - (c)) + ((b) - (d)) * ((b) - (d)))

double check(int car_id, int pos, int dest, int d4) {
	Car& car = cars[car_id];
	car.order[0] = car.pos.id;
	car.order[1] = pos;
	int d2 = gpt_search_catch(car.pos.id, pos);
	if (car.np == 0) {
		car.order[2] = dest;
		car.dist = d2;
		car.detour0 = 0;
		car.detour1 = 0;
		return d2;
	}
	int dests[4], from_car[4], from_psg[4], dis[4][4], d1 = 0x3fffffff, d3 = 0x3fffffff, pid = -1;
	for (int i = 0; i < car.np; ++i)
		dests[i] = car.pass[i].id;
	dests[car.np] = dest;
	for (int i = 0; i <= car.np; ++i) {
		from_car[i] = gpt_search_catch(car.pos.id, dests[i]);
		from_psg[i] = gpt_search_catch(pos, dests[i]);
		for (int j = 0; j < i; ++j)
			dis[i][j] = dis[j][i] = gpt_search_catch(dests[i], dests[j]);
	}
	if (car.np == 1) {
		d1 = from_car[0];
		d3 = std::min(from_psg[0], from_psg[1]) + dis[0][1];
		pid = from_psg[0] < from_psg[1] ? 0 : 1;
		for (int i = 0; i <= car.np; ++i)
			car.order[i + 2] = dests[p2[pid][i]];
	} else if (car.np == 2) {
		d1 = std::min(from_car[0], from_car[1]) + dis[0][1];
		for (int pidx = 0; pidx < 6; ++pidx) {
			int sum = from_psg[p3[pidx][0]];
			for (int i = 0; i <= car.np; ++i)
				sum += dis[p3[pidx][i]][p3[pidx][i + 1]];
			if (sum < d3) {
				d3 = sum;
				pid = pidx;
			}
		}
		for (int i = 0; i <= car.np; ++i)
			car.order[i + 2] = dests[p3[pid][i]];
	} else if (car.np == 3) {
		for (int pidx = 0; pidx < 6; ++pidx) {
			int sum = from_car[p3[pidx][0]];
			for (int i = 0; i < car.np; ++i)
				sum += dis[p3[pidx][i]][p3[pidx][i + 1]];
			if (sum < d1) d1 = sum;
		}
		for (int pidx = 0; pidx < 24; ++pidx) {
			int sum = from_psg[p4[pidx][0]];
			for (int i = 0; i <= car.np; ++i)
				sum += dis[p4[pidx][i]][p4[pidx][i + 1]];
			if (sum < d3) {
				d3 = sum;
				pid = pidx;
			}
		}
		for (int i = 0; i <= car.np; ++i)
			car.order[i + 2] = dests[p4[pid][i]];
	}
	car.dist = d2;
	car.detour0 = d2 + d3 - d1;
	car.detour1 = d3 - d4;
	if (car.detour0 <= 10000 && car.detour1 <= 10000)
		return car.dist + car.detour0 * .4 + car.detour1 * .6;
	else
		return -1;
}

extern "C" {

char s[60];
std::string output;

void init() {
	gpt_build();
	read_node();
	read_car();
	all_cars.clear();
	all_cars_pos.clear();
	all_cars.reserve(nCars);
	all_cars_pos.reserve(nCars);
	for (int i = 0; i < nCars; ++i)
		if (cars[i].np < 4 && cars[i].np > -1) {
			all_cars.push_back(i);
			all_cars_pos.push_back(cars[i].pos.id);
		}
	printf("avail cars: %d\n", all_cars.size());
}

char* get_pos(double lng, double lat) {
	int n = coords.size();
	int id = 0;
	double d = dis(coords[id].lng, coords[id].lat, lng, lat);
	for (int i = 0; i < n; ++i)
		if (dis(coords[i].lng, coords[i].lat, lng, lat) < d) {
			d = dis(coords[i].lng, coords[i].lat, lng, lat);
			id = i;
		}
	if (d >= 1e-5)
		return "{\"result\":[]}";
	sprintf(s, "{\"result\":[%d,%lf,%lf]}", id, coords[id].lng, coords[id].lat);
	return s;
}

char *query(int pos, int dest) {
	// select first 100th nearby cars
	std::vector<int> &&nearby_car = gpt_KNN(pos, 100, all_cars_pos);
	printf("Near cars: %d\n", nearby_car.size());
	int d4 = gpt_search_catch(pos, dest);
	// verify each taxi
	std::vector<CarItem> result;
	result.reserve(100);
	for (int idx : nearby_car) {
		int car_id = all_cars[idx];
		double cost = check(car_id, pos, dest, d4);
		if (cost >= 0)
			result.emplace_back((CarItem) {car_id, cost}); 
	}
	std::sort(result.begin(), result.end());
	printf("Result cars: %d\n", result.size());

	std::vector<int> path;
	output = "{\"result\":["
			+ std::to_string(pos) + ','
			+ std::to_string(coords[pos].lng) + ','
			+ std::to_string(coords[pos].lat) + ','
			+ std::to_string(dest) + ','
			+ std::to_string(coords[dest].lng) + ','
			+ std::to_string(coords[dest].lat) + ',';
	for (int i = 0; i < 5 && i < result.size(); ++i) {
		int car_id = result[i].id;
		Car &car = cars[car_id];
		output += std::string("[")
				+ std::to_string(car_id) + ','
				+ std::to_string(car.np + 3) + ','
				+ std::to_string(car.dist) + ','
				+ std::to_string(car.detour0) + ','
				+ std::to_string(car.detour1) + ",[";
		for (int j = 0; j < car.np + 2; ++j) {
			path.clear();
			gpt_find_path(car.order[j], car.order[j + 1], path);
			output += '[';
			for (int _ = 0; _ < path.size(); ++_) {
				output += '['
						+ std::to_string(path[_]) + ','
						+ std::to_string(coords[path[_]].lng) + ','
						+ std::to_string(coords[path[_]].lat) + ']'
						+ ",]"[_ == path.size() - 1];
			}
			output += ",]"[j == car.np + 1];
		}
		output += ']';
		if (i != 4 && i != result.size() - 1)
			output += ",";
	}
	output += "]}";
	return (char*) output.c_str();
}

}