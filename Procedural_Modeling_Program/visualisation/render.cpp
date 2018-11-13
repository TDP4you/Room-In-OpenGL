#include "render.h"

namespace visualisation
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow *window);

	// settings
	const unsigned int SCR_WIDTH = 1280;
	const unsigned int SCR_HEIGHT = 720;

	// camera
	Camera camera(glm::vec3(0.0f, 5.0f, 20.0f));
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	render::render(Shape* room) {
		this->room = room;
		getSize();
		visualise();
	}

	render::~render()
	{
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteVertexArrays(1, &lightVAO);
		glDeleteVertexArrays(1, &skyboxVAO);
		glDeleteBuffers(1, &skyboxVBO);
		glDeleteBuffers(1, &cubeVBO);
		cout << "Successfully deleted Buffers" << endl;
	}

	void render::getSize() {
		for (int i = 0; i < room->children.size(); ++i) {
			if (room->children[i]->getType() == "floor") {
				w = room->children[i]->getWidth();
				l = room->children[i]->getLength();
			}
			else if (room->children[i]->getType() == "wall") {
				h = room->children[i]->getLength();
			}
		}
	}

	void render::loadFurnitures() {
		for (int i = 0; i < room->children.size(); ++i)
		{
			if (room->children[i]->getType() == "floor")
			{
				for (int j = 0; j < room->children[i]->children.size(); ++j)
				{
					modelPosition.push_back(room->children[i]->children[j]->getPosition());
					modelScale.push_back(room->children[i]->children[j]->getScale());
					modelAngle.push_back(room->children[i]->children[j]->getAngle());
					modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
					Model mod(room->children[i]->children[j]->getModelName());
					models.push_back(mod);
				}
			}
		}
		//cout << " Models has this value " << models.data() << endl;
	}
	//Vertices
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	void render::glfwinitializer()
	{
		glfwWindowHint(GLFW_SAMPLES, 4); //Antialiasing
		glfwInit();
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Graphics Lab Final Project : Room", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
		glEnable(GL_MULTISAMPLE); //enabling Antialiasing
		glEnable(GL_DEPTH_TEST);
		cout << "Successfully Created Window" << endl;
	}

	void render::loadDoorAndWindow(){
		// set up vertex data (and buffer(s)) and configure vertex attributes
	    // ------------------------------------------------------------------
	    float back[] = {
	        //Door
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
	    };
	    float front[] = {
	        //Front side
	        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
	    };
	    float left[] = {
	        //Left side
	        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
	    };
	    float right[] = {
	        //Right side
	         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
	    };
		for (int i = 0; i < room->children.size(); ++i)
		{
			if (room->children[i]->getType() == "wall" && room->children[i]->children.size() > 0)
			{
				for (int j = 0; j < room->children[i]->children.size() ; ++j)
				{
					if (room->children[i]->children[j]->getType() == "door")
					{
						if (room->children[i]->getName() == "fwall")
						{
							modelPosition.push_back(glm::vec3(w / 4,-h/2, (l / 2) - 0.01));
							modelScale.push_back(glm::vec3(3.0f, 3.0f, 3.0f));
							modelAngle.push_back(glm::radians(0.0f));
							modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
							Model mod(room->children[i]->children[j]->getModelName());
							models.push_back(mod);
						} else if (room->children[i]->getName() == "bwall")
						{
							modelPosition.push_back(glm::vec3(w / 4, -h/2, (-l / 2) + 0.01));
							modelScale.push_back(glm::vec3(3.0f, 3.0f, 3.0f));
							modelAngle.push_back(glm::radians(0.0f));
							modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
							Model mod(room->children[i]->children[j]->getModelName());
							models.push_back(mod);
						} else if (room->children[i]->getName() == "lwall")
						{
							modelPosition.push_back(glm::vec3(-w / 2, -h/2, -l / 4));
							modelScale.push_back(glm::vec3(3.0f, 3.0f, 3.0f));
							modelAngle.push_back(90);
							modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
							Model mod(room->children[i]->children[j]->getModelName());
							models.push_back(mod);
						} else if (room->children[i]->getName() == "rwall")
						{
							modelPosition.push_back(glm::vec3(w / 2, -h / 2, - l / 4));
							modelScale.push_back(glm::vec3(3.0f, 3.0f, 3.0f));
							modelAngle.push_back(90);
							modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
							Model mod(room->children[i]->children[j]->getModelName());
							models.push_back(mod);
						}
					} else if (room->children[i]->children[j]->getType() == "window")
					{
						if (room->children[i]->getName() == "fwall")
						{
							string temp = room->children[i]->children[j]->getModelName();
							//cout << "Temp is :" << temp << endl;
							if (temp.compare(window1) == 0)
							{
								modelPosition.push_back(glm::vec3((w / 4) - 0.01, -(h / 2 - (h*0.7) / 2), l / 2));
								modelScale.push_back(glm::vec3(0.002f, 0.002f, 0.002f));
								modelAngle.push_back(glm::radians(0.0f));
								modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
								Model mod(room->children[i]->children[j]->getModelName());
								models.push_back(mod);
							}
							else if (temp.compare(window2) == 0)
							{
								if (temp.compare(window2) == 0)
								{
									modelPosition.push_back(glm::vec3((-w / 4) - 0.01, -(h / 2 - (h*0.7) / 2), l / 2));
									modelScale.push_back(glm::vec3(0.002f, 0.002f, 0.002f));
									modelAngle.push_back(glm::radians(0.0f));
									modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
									Model mod(room->children[i]->children[j]->getModelName());
									models.push_back(mod);
								}
							}
							else
							{
								cout << "Window Loading Failure" << endl;
							}
						}
						else if (room->children[i]->getName() == "bwall")
						{
							string temp = room->children[i]->children[j]->getModelName();
							//cout << "Temp is :" << temp << endl;
							if (temp.compare(window1) == 0)
							{
								modelPosition.push_back(glm::vec3((w / 4) - 0.01, -(h / 2 - (h*0.7) / 2), -l / 2));
								modelScale.push_back(glm::vec3(0.002f, 0.002f, 0.002f));
								modelAngle.push_back(glm::radians(0.0f));
								modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
								Model mod(room->children[i]->children[j]->getModelName());
								models.push_back(mod);
							}
							else if (temp.compare(window2) == 0)
							{
								if (temp.compare(window2) == 0)
								{
									modelPosition.push_back(glm::vec3((-w / 4) - 0.01, -(h / 2 - (h*0.7) / 2), -l / 2));
									modelScale.push_back(glm::vec3(0.002f, 0.002f, 0.002f));
									modelAngle.push_back(glm::radians(0.0f));
									modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
									Model mod(room->children[i]->children[j]->getModelName());
									models.push_back(mod);
								}
							}
							else
							{
								cout << "Window Loading Failure" << endl;
							}
						} else if (room->children[i]->getName() == "lwall")
						{
							string temp = room->children[i]->children[j]->getModelName();
							//cout << "Temp is :" << temp << endl;
							if (temp.compare(window1) == 0)
							{
								modelPosition.push_back(glm::vec3((-w / 2) + 0.01, -(h / 2 - (h*0.7) / 2), -l / 4));
								modelScale.push_back(glm::vec3(0.002f, 0.002f, 0.002f));
								modelAngle.push_back(90.0);
								modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
								Model mod(room->children[i]->children[j]->getModelName());
								models.push_back(mod);
							}
							else if (temp.compare(window2) == 0)
							{
								if (temp.compare(window2) == 0)
								{
									modelPosition.push_back(glm::vec3((-w / 2) + 0.01, -(h / 2 - (h*0.7) / 2), l/4));
									modelScale.push_back(glm::vec3(0.002f, 0.002f, 0.002f));
									modelAngle.push_back(90.0);
									modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
									Model mod(room->children[i]->children[j]->getModelName());
									models.push_back(mod);
								}
							}
							else
							{
								cout << "Window Loading Failure" << endl;
							}
						} else if (room->children[i]->getName() == "rwall")
						{
							string temp = room->children[i]->children[j]->getModelName();
							//cout << "Temp is :" << temp << endl;
							if (temp.compare(window1) == 0)
							{
								modelPosition.push_back(glm::vec3((w / 2) + 0.01, -(h / 2 - (h*0.7) / 2), -l / 4));
								modelScale.push_back(glm::vec3(0.002f, 0.002f, 0.002f));
								modelAngle.push_back(-90.0);
								modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
								Model mod(room->children[i]->children[j]->getModelName());
								models.push_back(mod);
							}
							else if (temp.compare(window2) == 0)
							{
								if (temp.compare(window2) == 0)
								{
									modelPosition.push_back(glm::vec3((w / 2) + 0.01, -(h / 2 - (h*0.7) / 2), l / 4));
									modelScale.push_back(glm::vec3(0.002f, 0.002f, 0.002f));
									modelAngle.push_back(-90.0);
									modelRotationVector.push_back(room->children[i]->children[j]->getRotationVector());
									Model mod(room->children[i]->children[j]->getModelName());
									models.push_back(mod);
								}
							}
							else
							{
								cout << "Window Loading Failure" << endl;
							}
						}
					}
				}
			}
		}

	}

	void render::loadRoom(){
		// set up vertex data (and buffer(s)) and configure vertex attributes
	    // ------------------------------------------------------------------
	    float wallVertices[] = {
	    	//Back side
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	        //Front side
	         //-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	         // 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	         // 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	         // 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	         //-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	         //-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	        //Left side
	        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	        //Right side
	         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	    };
	    float floorVertices[] = {
	         //Floor
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	    };
	    float ceilingVertices[] = {
	        //Ceiling
	        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
	    };

		bool wall=true;
		bool ceiling=true;
		for (int i = 0; i < room->children.size(); ++i)
		{
			if (room->children[i]->getType() == "ceiling" && ceiling)
			{
				unsigned int ceiling;
				roomTextures.push_back(ceiling);
				imgDir.push_back(room->children[i]->getTexture());
				roomPosition.push_back(glm::vec3( 0.0f,  0.0f,  0.0f));
				roomScale.push_back(glm::vec3(w, h, l));
				setRoomVertices(ceilingVertices, sizeof(ceilingVertices));
			
			} else if (room->children[i]->getType() == "floor")
			{
				unsigned int floor;
				roomTextures.push_back(floor);
				imgDir.push_back(room->children[i]->getTexture());
				roomPosition.push_back(glm::vec3( 0.0f,  0.0f,  0.0f));
				roomScale.push_back(glm::vec3(w, h, l));
				setRoomVertices(floorVertices, sizeof(floorVertices));
				
			} else if (room->children[i]->getType() == "wall" && wall)
			{
				unsigned int wall;
				roomTextures.push_back(wall);
				imgDir.push_back(room->children[i]->getTexture());
				roomPosition.push_back(glm::vec3( 0.0f,  0.0f,  0.0f));
				roomScale.push_back(glm::vec3(w, h, l));
				setRoomVertices(wallVertices, sizeof(wallVertices));
				wall=false;
			}
		}
		cout << "Room Loading Successful" << endl;
	}

	void render::setRoomVertices(float vertices[], int size){

		unsigned int VBO, VAO;
	    glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VBO);

	    glBindVertexArray(VAO);

	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW);

	    // position attribute
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(0);
	    // vertex normals	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
	    // texture coord attribute
	    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	    glEnableVertexAttribArray(2);

	    roomVAO.push_back(VAO);
	}

	void render::loadTextures(Shader shader){
		shader.use();
		for (int i = 0; i < roomTextures.size(); ++i)
		{
			// load and create a texture 
		    // -------------------------
		    glGenTextures(1, &roomTextures[i]);
		    glBindTexture(GL_TEXTURE_2D, roomTextures[i]);
		    // set the texture wrapping parameters
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		    // set texture filtering parameters
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    // load image, create texture and generate mipmaps
		    int width, height, nrChannels;
			cout << "Loading Texture from the following path : " << imgDir[i];
		    unsigned char *data = SOIL_load_image(imgDir[i], &width, &height, 0, SOIL_LOAD_RGB);
		    if (data)
		    {
		        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		        glGenerateMipmap(GL_TEXTURE_2D);
				std::cout << ". Texture Successfully loaded" << std::endl;
		    }
		    else
		    {
		        std::cout << "Failed to load texture" << std::endl;
		    }
		    SOIL_free_image_data(data);
		    shader.setInt("room", i);
		}
	}

	void render::initializeVertex()
	{
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);
		// we only need to bind to the cubeVBO, the container's cubeVBO's data already contains the correct data.
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		// set the vertex attributes (only position data for our lamp)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		cout << "Successfully initialized vertexes" << endl;
	}

	GLuint render::loadCubemap(vector<std::string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				SOIL_free_image_data(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				SOIL_free_image_data(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		cout << "Successfully loaded cubemap" << endl;
		return textureID;
	}

	void render::setLampPosition()
	{
		lampPosition.push_back(glm::vec3(w / 4, h / 2, l / 4));
		lampPosition.push_back(glm::vec3(w / 4, h / 2, -l / 4));
		lampPosition.push_back(glm::vec3(-w / 4, h / 2, l / 4));
		lampPosition.push_back(glm::vec3(-w / 4, h / 2, -l / 4));
	}

	void render::setLightPosition()
	{
		lightPosition.push_back(glm::vec3(w / 4, h / 2 - 0.7, l / 4));
		lightPosition.push_back(glm::vec3(w / 4, h / 2 - 0.7 , -l / 4));
		lightPosition.push_back(glm::vec3(-w / 4, h / 2 - 0.7 , l / 4));
		lightPosition.push_back(glm::vec3(-w / 4, h / 2 - 0.7, -l / 4));
	}
	void render::visualise(){
		glfwinitializer();

	    // build and compile shaders
	    // -------------------------
		Shader ourShader("shaders/mainvertex.vs", "shaders/mainfragment.fs");//Lightning Shader
		Shader lampShader("shaders/lightvertex.vs", "shaders/lightfragment.fs"); //Light Shader
		Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs"); //CubeMap Shader

	    // load room
	    // ---------
		loadRoom();
		loadDoorAndWindow();
		setLampPosition();
		setLightPosition();
		//initiliaze vertex
		initializeVertex();
		//load cubemap faces
		cubemapTexture = loadCubemap(faces);
	    
		// load and create a texture 
	    // -------------------------
	    loadTextures(ourShader);
	    
	    // load models
	    // -----------
		Model lamp("resources/Light/light.obj");
		loadFurnitures();
		
		ourShader.use();
		ourShader.setInt("material.diffuse", 0);
		ourShader.setInt("material.specular", 1);
		skyboxShader.use();
		skyboxShader.setInt("skybox", 0);

	    // render loop
	    // -----------
	    while (!glfwWindowShouldClose(window))
	    {
	        // per-frame time logic
	        // --------------------
	        float currentFrame = glfwGetTime();
	        deltaTime = 2*(currentFrame - lastFrame);
	        lastFrame = currentFrame;

	        // input
	        // -----
	        glfwPollEvents();
	        processInput(window);

	        // render
	        // ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ourShader.use();
			ourShader.setVec3("viewPos", camera.Position);
			ourShader.setFloat("material.shininess", 64.0f);
			//light properties
			ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
			ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
			// point light 1
			ourShader.setVec3("pointLights[0].position", lightPosition[0]);
			ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
			ourShader.setFloat("pointLights[0].constant", 1.0f);
			ourShader.setFloat("pointLights[0].linear", 0.09);
			ourShader.setFloat("pointLights[0].quadratic", 0.032);
			// point light 2
			ourShader.setVec3("pointLights[1].position", lightPosition[1]);
			ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
			ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
			ourShader.setFloat("pointLights[1].constant", 1.0f);
			ourShader.setFloat("pointLights[1].linear", 0.09);
			ourShader.setFloat("pointLights[1].quadratic", 0.032);
			// point light 2
			ourShader.setVec3("pointLights[2].position", lightPosition[2]);
			ourShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
			ourShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
			ourShader.setFloat("pointLights[2].constant", 1.0f);
			ourShader.setFloat("pointLights[2].linear", 0.09);
			ourShader.setFloat("pointLights[2].quadratic", 0.032);
			// point light 3
			ourShader.setVec3("pointLights[3].position", lightPosition[3]);
			ourShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
			ourShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
			ourShader.setFloat("pointLights[3].constant", 1.0f);
			ourShader.setFloat("pointLights[3].linear", 0.09);
			ourShader.setFloat("pointLights[3].quadratic", 0.032);

			ourShader.use();	        

	        // Transformation matrices
	        // -----------------------
			glm::mat4 projection(1);
	        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 1000.0f);
			glm::mat4 view(1);
			view = camera.GetViewMatrix();
	        ourShader.setMat4("projection", projection);
	        ourShader.setMat4("view", view);

	        
	        // draw room
	        // ---------
	        for (int i = 0; i < roomTextures.size(); ++i)
        	{
        		// bind textures on corresponding texture units
	        	glActiveTexture(GL_TEXTURE+i);
	        	glBindTexture(GL_TEXTURE_2D, roomTextures[i]);
		        // render boxes
	        	glBindVertexArray(roomVAO[i]);
	        	// calculate the model matrix for each object and pass it to shader before drawing
	        	glm::mat4 model(1);
	        	model = glm::translate(model, roomPosition[i]);
	        	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
	        	model = glm::scale(model, roomScale[i]);
	        	ourShader.setMat4("model", model);

	        	glDrawArrays(GL_TRIANGLES, 0, 18);		        	
        	}
			
        	// draw furnitures
        	// ---------------
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);
        	for (int i = 0; i < models.size(); ++i)
        	{
        		glm::mat4 modelObj(1);
        		modelObj = glm::translate(modelObj, modelPosition[i]); // Translate it down a bit so it's at the center of the scene
				modelObj = glm::rotate(modelObj, glm::radians((float) modelAngle[i]), glm::vec3(0.0f, 1.0f, 0.0f));
				modelObj = glm::scale(modelObj, modelScale[i]);	// It's a bit too big for our scene, so scale it down
				ourShader.setMat4("model", modelObj);
				models[i].Draw(ourShader);
				//cout << "Drawing Objects" << endl;
        	}
			//lamp objecct position
			for (int i = 0; i < lampPosition.size(); i++)
			{
				glm::mat4 model(1);
				model = glm::translate(model, lampPosition[i]); // translate it down so it's at the center of the scene
				model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
				ourShader.setMat4("model", model);
				lamp.Draw(ourShader);
			}
			//lamp white light	
			lampShader.use();
			lampShader.setMat4("projection", projection);
			lampShader.setMat4("view", view);
			glBindVertexArray(lightVAO);
			for (unsigned int i = 0; i < 4; i++)
			{
				glm::mat4 model(1);
				model = glm::translate(model, lightPosition[i]);
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::scale(model, glm::vec3(1.0f)); // a smaller cube
				lampShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			
			// draw skybox as last
			glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
			skyboxShader.use();
			view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
			skyboxShader.setMat4("view", view);
			skyboxShader.setMat4("projection", projection);
			// skybox cube
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS); // set depth function back to default

	        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	        // -------------------------------------------------------------------------------
	        glfwSwapBuffers(window);
	        glfwPollEvents();
	    }

	    // optional: de-allocate all resources once they've outlived their purpose:
	    // ------------------------------------------------------------------------
	    glDeleteVertexArrays(1, &wallVAO);
	    // glDeleteBuffers(1, &VBO);

	    glfwTerminate();

		}

		// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void processInput(GLFWwindow *window)
	{
	    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	        glfwSetWindowShouldClose(window, true);

	    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	        camera.ProcessKeyboard(FORWARD, deltaTime);
	    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	        camera.ProcessKeyboard(BACKWARD, deltaTime);
	    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	        camera.ProcessKeyboard(LEFT, deltaTime);
	    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	        camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
	    // make sure the viewport matches the new window dimensions; note that width and 
	    // height will be significantly larger than specified on retina displays.
	    glViewport(0, 0, width, height);
	}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
	    if (firstMouse)
	    {
	        lastX = xpos;
	        lastY = ypos;
	        firstMouse = false;
	    }

	    float xoffset = xpos - lastX;
	    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	    lastX = xpos;
	    lastY = ypos;

	    camera.ProcessMouseMovement(xoffset, yoffset);
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
	    camera.ProcessMouseScroll(yoffset);
	}

}