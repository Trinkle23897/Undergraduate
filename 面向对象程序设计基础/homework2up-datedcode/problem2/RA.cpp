#include <bits/stdc++.h>
#include "RA.h"

ReviewerAssigner::ReviewerAssigner()
{
	srand(time(0));
}
/**
 * [ReviewerAssigner::choose description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * randomly choose three reviewer
 */
void ReviewerAssigner::choose()
{
	int x, y, z;
	for (int i = 0; i < total; ++i)
	{
		do {
			x = rand() % total;
		} while (x == i);
		do {
			y = rand() % total;
		} while (y == i || y == x);
		do {
			z = rand() % total;
		} while (z == i || z == x || z == y);
		reviewer[i][0] = x;
		reviewer[i][1] = y;
		reviewer[i][2] = z;
	}
}
/**
 * [ReviewerAssigner::output description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * output the result
 */
void ReviewerAssigner::output()
{
	std::cerr << "ReviewerAssigner is done!" << std::endl;
	std::ofstream out("reviewer.txt");
	for (int i = 0; i < total; ++i)
	{
		out << stu[i];
		out << "\t Reviewer 1: " << stu[reviewer[i][0]].id << std::endl;
		out << "\t Reviewer 2: " << stu[reviewer[i][1]].id << std::endl;
		out << "\t Reviewer 3: " << stu[reviewer[i][2]].id << std::endl;
		out << std::endl;
	}
}
