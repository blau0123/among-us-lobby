#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "LightSource.h"
#include "Sphere.h"

#include "Transform.h"
#include "Geometry.h"

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// 1 --> rotate model, 2 --> rotate light source, 3 --> rotate both by same amount
	static int rotateType;

	// Objects to Render
	static Cube* cube;
	static Sphere* sphere;
	static PointCloud * cubePoints;
	static PointCloud* bunnyPoints;
	static PointCloud* sandalPoints;
	static PointCloud* bearPoints;
	static LightSource* lightSphere;

	// Scene Graph nodes
	static Transform* World;
	static Transform* GroundToWorld;
	static Transform* CylinderToGround;
	static Transform* WheelToCylinder;
	static std::vector<Transform*> CarsToWheel;

	static Geometry* Ground;
	static Geometry* WheelGeo;
	static Geometry* CylinderGeo;
	static std::vector<Geometry*> Cars;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 scale;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;
	static GLuint skyBoxShaderProgram;
	static GLuint sphereShaderProgram;

	static unsigned int cubemapTextureID;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static bool initializeSceneGraph();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void onMouseButtonDown(GLFWwindow* window, int button, int action, int mods);
	static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
};

#endif
