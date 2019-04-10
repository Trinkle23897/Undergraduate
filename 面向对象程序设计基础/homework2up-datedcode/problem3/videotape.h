#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <string>

class Video
{
public:
	std::string name;
	bool is_out;
	std::string customer;
	Video(std::string name);
	void rent(std::string customer);
	void back(std::string customer);
};

class Stack
{
	std::vector<Video> v;
public:
	void add(std::string name);
	void print();
	void require(std::string name, std::string customer);
	void sendback(std::string name, std::string customer);
};

#endif //__VIDEO_H__

