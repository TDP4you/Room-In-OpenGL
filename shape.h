#ifndef SHAPE_H
#define SHAPE_H


#include <string>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
using namespace std;

class shape
{
private:
	// --- Variables ----------------------		
	std::string Name;
	std::string Type;
	const char* Texture;
	std::string ModelName;
	glm::vec3 scale;
	glm::vec3 position;
	double Angle;	
	double Length;//for walls, floor and ceiling
	double Width;//for walls, floor and ceiling
	vector<shape*> Pair;

public:
	// --- Variables ----------------------
	std::vector<shape*> children;
		
	// --- Methods ------------------------	
	void setName(string name){Name = name;}	
	string getName(){return Name;}	
	void setType(string type){Type = type;}
	string getType(){return Type;}
	void setTexture(const char* texture) { Texture = texture; }
	const char* getTexture() { return Texture; }
	void setModelName(std::string model) { ModelName = model; }
	std::string getModelName(){ return ModelName; }
	void setScale(double sx, double sy, double sz){scale.x = sx;scale.y = sy;scale.z = sz;}
	glm::vec3 getScale() { return scale; }
	void setPosition(double sx, double sy, double sz){position.x = sx;position.y = sy;position.z = sz;}	
	glm::vec3 getPosition(){return position;}
	void setAngle(double angle) { Angle = angle; }
	double getAngle(){return Angle;}
	void setLength(double length){Length = length;}
	double getLength(){return Length;}
	void setWidth(double width){Width = width;}
	double getWidth(){return Width;}
	void setPair(shape* s){Pair.push_back(s);}
	vector<shape*> getPair(){return Pair;}

	shape()
	{
		setAngle(0);
		setPosition(0, 0, 0);
		setScale(1, 1, 1);
	}

	shape(std::string name, std::string type) :shape()
	{ 
		Name = name;
		Type = type;
		if (type == "furniture")
		{
			setPair(this);
		}
		
	}
	~shape(){}
};

#endif