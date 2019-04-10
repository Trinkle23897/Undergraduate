#include <bits/stdc++.h>
#include "videotape.h"

int main(int argc, char const *argv[])
{
	Stack s;
	int cas = 0;
	while (1)
	{
		std::cout << "0: Add new video\n1: Rent video\n2: Send back video\n3: Print video list\n4: Quit" << std::endl;
		std::cin >> cas;
		if (cas == 4)
		{
			std::cout << "Your GPA will be 4.0!" << std::endl;
			return 0;
		}
		if (cas < 0 || cas > 4)
			continue;
		if (cas == 0)
		{
			std::cout << "Please input this new video's name: ";
			std::string str;
			std::cin >> str;
			s.add(str);
		}
		else if (cas == 1)
		{
			std::cout << "Please input this video's name: ";
			std::string str;
			std::cin >> str;
			std::cout << "Please input customer's name: ";
			std::string name;
			std::cin >> name;
			s.require(str, name);
		}
		else if (cas == 2)
		{
			std::cout << "Please input this video's name: ";
			std::string str;
			std::cin >> str;
			std::cout << "Please input customer's name: ";
			std::string name;
			std::cin >> name;
			s.sendback(str, name);
		}
		else
			s.print();
	}
	return 0;
}
