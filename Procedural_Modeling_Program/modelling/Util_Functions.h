#pragma once
#include <iostream>
#include <vector>

static std::string copy_string(std::string a)
{
	int size = a.size();
	std::string b(size, '0');
	for (int i = 0; i < size; i++)
	{
		b.at(i) = a.at(i);
	}
	return b;
}

static std::string trim_string(std::string a)
{
	std::string b = "";
	int size = a.size();
	for (int i = 0; i < size; i++)
	{
		if (a.at(i) != ' ')
		{
			b += a.at(i);
		}
	}
	return b;
}

static std::vector<std::string> split_string(std::string a, std::string character)
{
	std::vector<std::string> b;
	int pos = a.find(character);
	b.push_back(a.substr(0, pos));
	b.push_back(a.substr(pos + character.size()));
	return b;
}