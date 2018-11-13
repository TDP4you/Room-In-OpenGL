#include <vector>
#include <utility>
#include<glm\glm.hpp>

#pragma once

class Shape {
private:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotationangle;
	double angle;
	double width;
	double length;
	double height;
    //std::vector<int> objectLW;
	std::string ModelName;
	std::string text;
	const char* Texture;
	std::string Name;
	std::string Type;
	std::vector<Shape *> pairwise_distances;
	Shape* parent;


public:
	std::vector<Shape*>children;
	//Constructor
	Shape();
	Shape(std::string n, std::string t);
	Shape(double x, double y, double z, double width, double length, double height);
	~Shape();
	void ShapeObject(double x, double y, double z, double width, double length, double height);
    //void ShapeObject(std::vector<int> center);

	//function to get all vertices
    //std::vector<std::vector<int>> getObjectVertices();

	//get set functions for center coordinates
	void setPosition(double x, double y, double z);
    glm::vec3 getPosition();

	//get set functions for scale
	void setScale(double x, double y, double z);
	glm::vec3 getScale();
	void setRotationVector(double x, double y, double z);
	glm::vec3 getRotationVector();
    double getWidth();
	void setWidth(double w);
	double getLength();
	void setLength(double l);
	double getHeight();
	void setHeight(double h);
	void setAngle(double a);
	double getAngle();

    //void updateCenterCoordinate(std::vector<int> object);

	//get set functions for symbol 
	void setType(std::string name);
	std::string getType();

	//get set functions for name 
	void setName(std::string name);
	std::string getName();

	//get set function for mtl filename
	void setTexture(const char*);
	const char* getTexture();

	//get set function for obj filename
	void setModelName(std::string);
	std::string getModelName();

	//get set function for pairwisedistance
	void addPairWiseDistance(Shape* s);
	std::vector<Shape*> getPairwiseDistances();

	void setParent(Shape* a);
	Shape* getParent();

	void addChild(Shape* a);
	std::vector<Shape*> getChildren();

	//function for debug purposes, prints all attribute values
	void debug();
};

