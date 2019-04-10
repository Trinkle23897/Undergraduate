#include <bits/stdc++.h>
#include "student.h"
/**
 * [student::init description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @param    str        original information in this string
 * change info in str into id & email
 */
void student::init(std::string str)
{
	std::stringstream ss(str);
	ss >> id >> email;
}
/**
 * reload the ofstream
 */
std::ostream& operator<<(std::ostream& out, const student& stu)
{
	out << "ID: " << stu.id << "\tEmail: " << stu.email << std::endl;
	return out;
}

Assigner::Assigner() { total = 0; }
/**
 * [Assigner::load description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * load info from the file.
 */
void Assigner::load()
{
	std::ifstream in("ContactEmail.txt");
	std::string tmp;
	std::getline(in, tmp);
	while (std::getline(in, tmp))
		stu[total++].init(tmp);
}
