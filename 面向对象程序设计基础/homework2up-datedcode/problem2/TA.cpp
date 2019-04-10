#include "TA.h"
#include <bits/stdc++.h>

TeamAssigner::TeamAssigner()
{
	group_total = 0;
	srand(time(0));
}
/**
 * [TeamAssigner::assign description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * begin with a random shuffle;
 * seperate team mainly in 3 people per group,
 * otherwise in 2 or 4
 */
void TeamAssigner::assign()
{
	memset(group_size, 0, sizeof group_size);
	for (int i = 0; i < total; ++i)
	{
		group[i] = i;
	}
	std::random_shuffle(group, group + total);
	for (; group_size[group_total] < total; group_total++)
	{
		group_size[group_total + 1] = group_size[group_total] + 3;
	}
	group_size[group_total] = total;
	if (total - group_size[group_total - 1] == 1)
		group_size[group_total - 1]--;
}
/**
 * [TeamAssigner::output description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * output the result
 */
void TeamAssigner::output()
{
	std::cerr << "TeamAssigner is done!" << std::endl;
	std::ofstream out("team.txt");
	for (int i = 0; i < group_total; ++i)
	{
		int j = group_size[i], k = group_size[i + 1];
		out << "Group " << i + 1 << ": " << k - j << " members" << std::endl;
		for (; j < k; ++j)
		{
			out << "\t" << stu[group[j]];
		}
		out << std::endl;
	}
}
