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

	/* Scene Graph nodes
		World -- translateGround -- translatePole -- translateWheel -- scaleSupport -- (translateSupport -- rotateSupportAboutPole -- support) repeat for each support pole
					|				  |					|		\
					|				  |					|		 \
					|				  |					|		   -- translateCar -- car
					|				  |					|						\
					|				  |					|						  -- scaleAttachToWheel -- attachToWheel
					|				  |					|
					|				  |					  -- scaleWheel -- wheel
					|				  | 
					|					-- scalePole -- pole
					|
					 -- translateGroundBack -- ground
	*/
	static Transform* World;
	static Transform* translateGround;
	static Transform* translateGroundBack;
	static Transform* translatePole;
	static Transform* scalePole;
	static Transform* translateWheel;
	static Transform* scaleWheel;
	static Transform* rotateSupportPole;
	static Transform* rotateCar;
	static Transform* scaleAttachPole;
	static std::vector<Transform*> translateCars;
	static std::vector<Transform*> rotateCars;

	static Geometry* ground;
	static Geometry* pole;
	static Geometry* wheel;
	static Geometry* supportPole;
	static Geometry* car;
	static Geometry* car2;
	static std::vector<Geometry*> cars;
	static std::vector<Geometry*> attachPoles;

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

	static bool createRideCars();
	static bool createAttachPoles();

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
