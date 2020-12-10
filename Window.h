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
#include "Lobby.h"

#include "Transform.h"
#include "Geometry.h"

#include <ctime>

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
	static bool is_W_down;
	static bool is_A_down;
	static bool is_S_down;
	static bool is_D_down;

	// Objects to Render
	static Cube* cube;
	static Sphere* sphere;
	static PointCloud * cubePoints;
	static PointCloud* bunnyPoints;
	static PointCloud* sandalPoints;
	static PointCloud* bearPoints;

	static LightSource* lightSphere;

	/*
	Scene graph for among us
	World -- rotateWorld -- scaleLobby -- lobby
						|
						-- translateAstronaut -- rotateAstronaut -- scaleAstronaut -- astronaut (for every astronaut)
	*/
	static Transform* World;
	static Transform* rotateWorld;
	static Transform* scaleLobby;
	static Transform* rotateLobby;
	static Transform* scaleUserAstronaut;
	static Transform* translateUserAstronaut;
	static Transform* rotateUserAstronaut;
	static Geometry* lobby;
	static Geometry* userAstronaut;

	// allAstronauts and allAstroTransforms will be in same order (allAstro[i] corresponds to the transform allAstroTranslates[i])
	static std::vector<Geometry*> allAstronauts;
	static std::vector<Transform*> allAstroTranslates;
	static std::vector<Transform*> allAstroRotates;

	// Records the direction that the user is currently facing/walking in (1.0f in the direction it's facing)
	static glm::vec3 userDirection;
	static std::vector<glm::vec3> allAstroDirections;

	// Vector of bounding spheres representing the obstacles (loop through to
	// check if colliding with an obstacle) --> obstacles = boundingspheres, walls = boundingplanes
	static std::vector<BoundingSphere*> obstacles;
	static std::vector<BoundingPlane*> walls;
	//static std::vector<BoundingSphere*> allAstronautsSpheres;

	static Geometry* testSphere;
	static Transform* transformSphere;

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

	// Trackball rotation
	static glm::vec3 lastCursorPos;
	// Determines if we are rotating the object or not --> 0 = ROTATE
	static int movement;

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
	static void initializeOtherAstronauts();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	static void updateCameraIfKeyHold();
	static void updatePlayerIfKeyHold(float collision);
	static glm::vec3 nonPlayerCollisionResolutionWithWall(Geometry* astro, glm::vec3 currDir, BoundingPlane* collidedWith, Transform* astroTranslate);
	//static Bounding* detectCollisions(Geometry* obj);
	static BoundingSphere* detectCollisionWithSphere(Geometry* obj);
	static BoundingPlane* detectCollisionWithWall(Geometry* obj);
	static float getCollisionAmount(BoundingSphere* obj, BoundingSphere* obj2);
	static float getCollisionAmountWithWall(BoundingSphere* obj, BoundingPlane* wall);

	static void updateAstronautDirection(glm::vec3 newDirection, glm::vec3 currDirection, Transform* rotateSpecificAstronaut);
	static std::vector<glm::vec3> getAstronautColors();

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void onMouseButtonDown(GLFWwindow* window, int button, int action, int mods);
	static void onMouseMove(GLFWwindow* window, double xpos, double ypos);

	// For trackball rotation of lobby about its center
	static void initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos);
	static std::pair<glm::vec3, float> rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos);
	static void endRotateModel();
	static glm::vec3 trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos);
};

#endif
