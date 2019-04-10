#ifndef __STU_H__
#define __STU_H__

#include <string>

class student
{
public:
	int id;
	std::string email;
	
	void init(std::string str);
};

std::ostream& operator<<(std::ostream& out, const student& stu);

class Assigner
{
public:
	int total;
	student stu[50];
	Assigner();
	void load();
};

#endif //__STU_H__

