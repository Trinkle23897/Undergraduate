#include <bits/stdc++.h>
#include "videotape.h"

Video::Video(std::string name)
{
	this->name = name;
	is_out = 0;
}
/**
 * [Video::rent description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @param    customer   people who tend to rent this video
 * if it hasn't been rent, then go though the procedure;
 * otherwise throw an error message.
 */
void Video::rent(std::string customer)
{
	if (is_out == 1)
	{
		std::cout << "This Video has been rent by " << this->customer << ", sorry!" << std::endl;
		return;
	}
	is_out = 1;
	this->customer = customer;
	std::cout << customer << " has successfully rent \"" << this->name << "\"!" << std::endl;
}
/**
 * [Video::back description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @param    customer   people who tend to send this video back
 * if the video is rented by this person, then go thourgh the procedure;
 * otherwise throw an error message.
 */
void Video::back(std::string customer)
{
	if (is_out == 0)
	{
		std::cout << "You don't need to return this video because it hasn't been rent yet!" << std::endl;
		return;
	}
	if (this->customer != customer)
	{
		std::cout << "You don't need to return this video because it has been rent by " << this->customer << "." << std::endl;
		return;
	}
	is_out = 0;
	std::cout << customer << " has successfully send it back!" << std::endl;
}
/**
 * [Stack::add description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @param    name       new video's name
 * add the new video into the stack
 */
void Stack::add(std::string name)
{
	Video d = Video(name);
	v.push_back(d);
}
/**
 * [Stack::print description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * print the detail message of all video in the stack
 */
void Stack::print()
{
	std::cout << std::endl;
	int cnt = 1;
	for (std::vector<Video>::iterator i = v.begin(); i != v.end(); ++i, ++cnt)
	{
		std::cout << "ID: " << cnt << "\tVideo name: \"" << (*i).name << "\",\t";
		if ((*i).is_out)
			std::cout << "has been rent by " << (*i).customer << "." << std::endl;
		else
			std::cout << "hasn't been rent." << std::endl;
	}
	std::cout << std::endl;
}
/**
 * [Stack::require description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @param    name       video's name
 * @param    customer
 * check if this customer can rent this video
 */
void Stack::require(std::string name, std::string customer)
{
	for (std::vector<Video>::iterator i = v.begin(); i != v.end(); ++i)
		if ((*i).name == name && (*i).is_out == 0)
		{
			(*i).rent(customer);
			return;
		}
	std::cout << "Sorry, we don't have this video." << std::endl;
}
/**
 * [Stack::sendback description]
 * @Author   n+e
 * @DateTime 2017-04-02
 * @param    name       video's name
 * @param    customer
 * check if this customer can send this video back
 */
void Stack::sendback(std::string name, std::string customer)
{
	for (std::vector<Video>::iterator i = v.begin(); i != v.end(); ++i)
		if ((*i).name == name && (*i).is_out == 1 && (*i).customer == customer)
		{
			(*i).back(customer);
			return;
		}
	std::cout << "There must be something wrong." << std::endl;
}
