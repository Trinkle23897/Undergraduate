#ifndef __ROUTE_RULE_BASED_H__
#define __ROUTE_RULE_BASED_H__

#include "../route.h"
#include <cmath>
#include <vector>

const double pi = acos(-1);

class RouteRuleBasedImpl : public Route
{
	typedef std::vector<int> IntList;
	typedef std::vector<IntList> VecList;
	typedef std::vector<char> ShortList;
	typedef std::vector<ShortList> VecList_short;

	int time_, total_error_node, interval, max_time;
	double center_x, center_y;
	VecList_short dir; //0123:normal 4: banned -1: avaliable 5: source
	VecList_short dir_bak;//original dir (only -1 & 5)
	VecList_short dir_tmp;//using when doing A*
	VecList_short dir_ans;
	VecList distance;//using when doing A*
	VecList visit;//using when doing A*
	VecList_short label;
	VecList_short label_bak;
	VecList_short label_bdr;
	VecList label_show;//priority in showing path
private:
	void rotate(double&x, double&y, double angle = 1e-3);
	double evaluate_to_edge(double x, double y);//calc min dist to edge
	int evaluate_to_point(int x, int y, Vertex T);//calc manhattan distance
	int best_direction(int x, int y);//return 0/1/2/3
	int calc_no_target_route(Vertex S);//find if there is a path from S to one edge
	int calc_route(Vertex S, Vertex T);//find a path from S to T, if it doesn't exist then return false. using A*
	int calc_midean(int*res, int n);
	bool label_dir(const int&check_case, Vertex S, Vertex T);//using in calc_route
	void calc_00_dir(const int case_ = 0); //calc 1/4 square's points' solution
	void calc_0_dir(const int case_ = 0); //change direction to 0 dir, same as below
	void calc_1_dir(const int case_ = 0);
	void calc_2_dir(const int case_ = 0);
	void calc_3_dir(const int case_ = 0);
	void cw_dir(VecList_short&dir_ans, VecList_short&dir);
	void ccw_dir(VecList_short&dir_ans, VecList_short&dir);
	Path compute_path(int x, int y, int _case);//extract path from dir_ans
	void init_array();
public:
	void compute(CallbackFunc);
};

#endif
