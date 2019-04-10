#ifndef __RA_H__
#define __RA_H__

#include "student.h"

class ReviewerAssigner: public Assigner
{
	int reviewer[50][3];
public:
	ReviewerAssigner();
	void choose();
	void output();
};

#endif //__RA_H__

