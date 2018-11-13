#ifndef RENDER_H
#define RENDER_H

// GLFW - glad - stb_image
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <SOIL/SOIL.h>

// GLM Mathemtics
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// GL includes
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "..\modelling\Shape.h"

#include <iostream>

namespace visualisation
{

	class render
	{
	private:
		string window1 = "resources/window1/window.obj";
		string window2 = "resources/window2/window.obj";
		GLFWwindow * window;
		GLuint cubemapTexture;
		Shape* room;
		GLuint wallVAO, floorVAO, ceilingVAO, doorVAO, windowVAO, cubeVBO, cubeVAO, lightVAO, skyboxVAO, skyboxVBO;
		vector<unsigned int> roomTextures;
		vector<unsigned int> roomVAO;
		vector<const char*> imgDir;
		vector<string> modelPath;
		vector<glm::vec3> modelPosition;
		vector<glm::vec3> modelRotationVector;
		vector<glm::vec3> modelScale;
		vector<double> modelAngle;
		vector<Model> models;
		vector<glm::vec3> roomPosition;
		vector<glm::vec3> roomScale;
		vector<glm::vec3> lampPosition;
		vector <glm::vec3> lightPosition;
		double w;
		double l;
		double h;
	public:
		render(Shape* room);
		~render();
		void getSize();
		void visualise();
		void loadFurnitures();
		void loadRoom();
		void setRoomVertices(float vertices[], int size);
		GLuint loadCubemap(vector<std::string> faces);
		void initializeVertex();
		void setLampPosition();
		void setLightPosition();
		void glfwinitializer();
		void loadDoorAndWindow();
		void loadTextures(Shader shader);
	
	};
}

#endif