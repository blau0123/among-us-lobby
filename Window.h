#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "LightSource.h"
#include "Sphere.h"
#include "AmongUsObject.h"

#include "Transform.h"
#include "Geometry.h"

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Texture properties
	static GLuint textureId;

	// 1 --> rotate model, 2 --> rotate light source, 3 --> rotate both by same amount
	static int rotateType;

	// anim1 = wheel translating up and down pole
	// anim2 = wheel rotating
	// anim3 = cars rotating
	static int anim1;
	static int anim2;
	static int anim3;
	// Determine whether the wheel should be translating up or down. Max = 14, min = 5
	static float counter;
	// If -1, then is currently going down, if 1, then is currently going up
	static int upOrDown;
	/*
	moving == -1 --> not moving
	moving == 0 --> forward
	moving == 1 --> backward
	moving == 2 --> up
	moving == 3 --> down
	moving == 4 --> turn left
	moving == 5 --> turn right
	*/
	static int moving;

	// Objects to Render
	static Cube* cube;
	static Sphere* sphere;
	static PointCloud * cubePoints;
	static PointCloud* bunnyPoints;
	static PointCloud* sandalPoints;
	static PointCloud* bearPoints;

	static AmongUsObject* lobby;
	static AmongUsObject* userAstronaut;

	static LightSource* lightSphere;

	/* Scene Graph nodes
		World -- translateGround -- translatePole -- rotateWheel -- translateWheel -- scaleSupport -- (translateSupport -- support) repeat for each support pole
					|				  |					|		\
					|				  |					|		 \
					|				  |					|		   -- translateCar -- rotateCar -- car
					|				  |					 |					\
					|				  |					|					  -- scaleAttachToWheel -- attachToWheel
					|				  |					|
					|				  |					  -- scaleWheel -- wheel
					|				  | 
					|					-- scalePole -- pole
					|
					 -- scaleGround -- translateGroundBack -- ground
	*/
	static Transform* World;
	static Transform* translateGround;
	static Transform* scaleGround;
	static Transform* translateGroundBack;
	static Transform* translatePole;
	static Transform* scalePole;
	static Transform* translateWheelUpAndDown;
	static Transform* translateWheel;
	static Transform* scaleWheel;
	static Transform* rotateWheel;
	static Transform* scaleSupportPoleLeft;
	static Transform* scaleSupportPoleRight;
	static Transform* scaleSupportPoleFront;
	static Transform* scaleSupportPoleBack;
	static Transform* rotateSupportPoleX;
	static Transform* rotateSupportPoleZ;
	static Transform* scaleAttachPole;
	static std::vector<Transform*> translateCars;
	static std::vector<Transform*> rotateCars;

	static Geometry* ground;
	static Geometry* pole;
	static Geometry* wheel;
	static Geometry* car;
	static Geometry* car2;
	static std::vector<Geometry*> cars;
	static std::vector<Geometry*> attachPoles;
	static std::vector<Geometry*> supportPoles;

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
	static bool createSupportPoles();

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

	static void updateCameraIfKeyHold();

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void onMouseButtonDown(GLFWwindow* window, int button, int action, int mods);
	static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
};

#endif
