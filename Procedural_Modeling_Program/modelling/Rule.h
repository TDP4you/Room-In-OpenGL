#pragma once
#include <iostream>
#include <string>

class Rule
{
private:
	int id;
	std::string predecessor;
	std::vector<Rule*> successors;
	std::vector<std::string> pairwise_distances;
	std::vector<float> center;
	std::vector<float> size;
	float rotatex;
	float rotatey;
	float rotatez;
	std::string obj_name;
	float prob;
	int priority;
	bool active;

public:
	Rule()
	{
		priority = 1;
		prob = 1;
		rotatex = 0;
		rotatey = 0;
		rotatez = 0;
		active = true;
	}

	void set_id(int i)
	{
		id = i;
	}

	int get_id()
	{
		return id;
	}

	void set_predecessor(std::string p)
	{
		predecessor = p;
	}

	void add_successors(Rule* a)
	{
		successors.push_back(a);
	}

	std::vector<Rule*> get_successors()
	{
		return successors;
	}

	void add_pairwise_distance(std::string a)
	{
		pairwise_distances.push_back(a);
	}

	std::vector<std::string> get_pairwise_distances()
	{
		return pairwise_distances;
	}

	void set_prob(float probability)
	{
		prob = probability;
	}

	float get_prob()
	{
		return prob;
	}

	void set_priority(int prior)
	{
		priority = prior;
	}

	int get_priority()
	{
		return priority;
	}

	std::string get_predecessor()
	{
		return predecessor;
	}

	void set_center(std::vector<float> c)
	{
		center = c;
	}

	std::vector<float> get_center()
	{
		return center;
	}

	void set_rotatex(float c)
	{
		rotatex = c;
	}

	float get_rotatex()
	{
		return rotatex;
	}

	void set_rotatey(float c)
	{
		rotatey = c;
	}

	float get_rotatey()
	{
		return rotatey;
	}

	void set_rotatez(float c)
	{
		rotatez = c;
	}

	float get_rotatez()
	{
		return rotatez;
	}

	void set_size(std::vector<float> c)
	{
		size = c;
	}

	std::vector<float> get_size()
	{
		return size;
	}

	void set_obj_name(std::string c)
	{
		obj_name = c;
	}

	std::string get_obj_name()
	{
		return obj_name;
	}

	bool is_active()
	{
		return active;
	}

	void set_active(bool a)
	{
		active = a;
	}

	void debug()
	{
		std::cout << "Rule: " << id
			<< " Predecessor: " << predecessor
			<< " Prob: " << prob
			<< " Priority: " << priority
			<< " Rotatex: " << rotatex
			<< " Rotatey: " << rotatey
			<< " Rotatez: " << rotatez
			<< " obj: " << obj_name
			<< " active: " << active
			<< std::endl;

		int size = successors.size();
		//std::cout << size << std::endl;
		for (int i = 0; i < size; i++)
		{
			std::cout << "Successor: " << successors.at(i)->get_predecessor() << std::endl;
		}

		if (center.size() > 0)
		{
			//std::cout << center.size() << std::endl;
			std::cout  << " Posx: " << center.at(0) << " Posy: " << center.at(1) << " Posz: " << center.at(2) << std::endl;
		}

		if (this->size.size()>0)
		{
			//std::cout << this->size.size() << std::endl;
			std::cout << " Sizex: " << this->size[0] << " Sizey: " << this->size[1] << " Sizez: " << this->size[2] << std::endl;
		}

		std::cout << "Pairwise Distances" << std::endl;
		size = pairwise_distances.size();
		for (int i = 0; i < size; i++)
		{
			std::cout <<i+1<< " Rule: " << pairwise_distances.at(i)<< std::endl;
		}
	}


};