#ifndef __TA_H__
#define __TA_H__

#include "student.h"

class TeamAssigner: public Assigner
{
	int group_size[30], group_total;
	int group[50];
public:
	TeamAssigner();
	void assign();
	void output();
};

#endif //__TA_H__

