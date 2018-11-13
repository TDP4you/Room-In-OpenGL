#include <fstream>
#include <iterator>
#include "Shape.h"
#include <iostream>
#include <string>

using namespace std;

Shape::Shape()
{

}

Shape::Shape(std::string n, std::string t)
{
	Name = n;
	if (t.find("wall")!=-1)
	{
		Type = "wall";
	}
	else if (t.find("floor") != -1)
	{
		Type = "floor";
	}
	else if (t.find("ceiling") !=-1)
	{
		Type = "ceiling";
	}

	else if (t.find("door") != -1)
	{
		Type = "door";
	}

	else if (t.find("window") != -1)
	{
		Type = "window";
	}

	else
	{
		Type = "furniture";
	}

	//Type = t;
	setPosition(0, 0, 0);
	setAngle(0);
	setScale(1, 1, 1);
	setModelName("");
	setTexture("");
	setRotationVector(0,1,0);
	//setModelName(Type);
	//setTexture(Type);
}

Shape::Shape(double x, double y, double z, double width, double length, double height)
{
	setPosition(x, y, z);
	setScale(width, length, height);
	setRotationVector(0,1,0);
	addPairWiseDistance(this);
}

Shape::~Shape()
{

}

//get set functions for center coordinates
void Shape::setPosition(double x, double y, double z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	//position = glm::vec3(x, y, z);
}

glm::vec3 Shape::getPosition()
{
	return position;
}

//get set functions for scale
void Shape::setScale(double x, double y, double z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}
glm::vec3 Shape::getScale()
{
	return scale;
}

void Shape::setRotationVector(double x, double y, double z)
{
	rotationangle.x = x;
	rotationangle.y = y;
	rotationangle.z = z;
}
glm::vec3 Shape::getRotationVector()
{
	return rotationangle;
}

double Shape::getWidth()
{
	return width;
}

void Shape::setWidth(double w)
{
	width = w;
}

void Shape::setHeight(double h)
{
	height = h;
}

void Shape::setLength(double l)
{
	length = l;
}

double Shape::getLength()
{
	return length;
}

double Shape::getHeight()
{
	return height;
}

void Shape::setAngle(double a)
{
	 angle=a;
}

double Shape::getAngle()
{
	return angle;
}

//get set functions for type 
void Shape::setType(std::string s)
{
	Type = s;
}

std::string Shape::getType()
{
	return Type;
}

//get set functions for name 
void Shape::setName(std::string name)
{
	this->Name = name;
}

std::string Shape::getName()
{
	return Name;
}

//get set function for texture
void Shape::setTexture(const char* t)
{
	Texture = t;
}

const char* Shape::getTexture()
{
	return Texture;
}

const char* get_const_from_string(std::string s)
{
	const char* kot = "";
	int size = s.size();
	for (int i = 0; i < size; i++)
	{
		kot += s[i];
	}
	kot += '\0';
	return kot;
}

//get set function for obj filename
void Shape::setModelName(std::string s)
{
	if (s.find(".obj") !=-1)
	{
		ModelName = s;
	}
	else
	{
		text = s;
		const char* pc = text.c_str();
		setTexture(pc);
	}
}

std::string Shape::getModelName()
{
	return ModelName;
}

//get set function for pairwisedistance
void Shape::addPairWiseDistance(Shape* a)
{
	pairwise_distances.push_back(a);
}

std::vector<Shape*> Shape::getPairwiseDistances()
{
	return pairwise_distances;
}

//function for debug purposes, prints all attribute values
void Shape::debug()
{
	cout <<"Shape " << Name.c_str() << endl
		<< "Type: " << Type.c_str() << " ModelName: " << ModelName.c_str() << endl <<"Position coordinates"<<endl;
	printf("Texture : %s", Texture);
	cout << endl;
    cout << "x: " << position.x << " y: " << position.y << " z: " << position.z << endl;
	cout << "width: " << width << " length: " << length << " height: " << height <<" angle: "<<angle<< endl;

	cout << "Pairwise Distance Relationship:" << endl;
	int size = pairwise_distances.size();
	//cout << size << endl;
	for (int i = 0; i < size; i++)
	{
		cout << pairwise_distances.at(i)->getName().c_str()<< endl;
	}
	if(parent!=nullptr)
	cout << "Parent: " << parent->getName().c_str() << endl;
	cout << "Children: " << endl;
	size = children.size();
	for (int i = 0; i < size; i++)
	{
		cout << i+1<<": "<<children.at(i)->getName().c_str()<< endl;
		//children.at(i)->debug();
	}

}

void Shape::setParent(Shape* a)
{
	parent = a;
}

Shape* Shape::getParent()
{
	return parent;
}

void Shape::addChild(Shape* a)
{
	children.push_back(a);
}
std::vector<Shape*> Shape::getChildren()
{
	return children;
}



