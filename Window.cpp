#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

GLuint Window::textureId;

// Animation controls (0 = no animation, 1 = animation)
int Window::anim1 = 1;
int Window::anim2 = 1;
int Window::anim3 = 1;

float Window::counter = 14.0f;
int Window::upOrDown = -1;
int Window::moving = -1;

// Trackball rotation
glm::vec3 Window::lastCursorPos;
int Window::movement = -1;

// For user movement
bool Window::is_W_down = false;
bool Window::is_A_down = false;
bool Window::is_S_down = false;
bool Window::is_D_down = false;

// Objects to Render
Cube * Window::cube;
Sphere* Window::sphere;
PointCloud * Window::cubePoints;
PointCloud* Window::bunnyPoints;
PointCloud* Window::sandalPoints;
PointCloud* Window::bearPoints;

// When render, will start by facing -z direction
glm::vec3 Window::userDirection(0.0f, 0.0f, -1.0f);

// Among us geometries and transforms
Geometry* Window::lobby;
Geometry* Window::userAstronaut;
std::vector<Geometry*> Window::allAstronauts;
std::vector<Transform*> Window::allAstroTranslates;
std::vector<Transform*> Window::allAstroRotates;

std::vector<BoundingSphere*> Window::obstacles;
std::vector<BoundingPlane*> Window::walls;
Geometry* Window::testSphere;

Transform* Window::transformSphere;
Transform* Window::rotateWorld;
Transform* Window::rotateLobby;
Transform* Window::scaleLobby;
Transform* Window::translateUserAstronaut;
Transform* Window::rotateUserAstronaut;
Transform* Window::scaleUserAstronaut;

LightSource* Window::lightSphere;
Object* currObj;

unsigned int Window::cubemapTextureID;

// Scene Graph nodes
Transform* Window::World;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// Scale matrix:
glm::mat4 Window::scale;

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

int Window::rotateType = 1;

// Shader Program ID
GLuint Window::shaderProgram; 
GLuint Window::skyBoxShaderProgram;
GLuint Window::sphereShaderProgram;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	skyBoxShaderProgram = LoadShaders("shaders/skyBoxShader.vert", "shaders/skyBoxShader.frag");
	sphereShaderProgram = LoadShaders("shaders/sphereShader.vert", "shaders/sphereShader.frag");

	// Check the shader program.
	if (!shaderProgram || !skyBoxShaderProgram || !sphereShaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeSceneGraph() {
	// Set up scene graph and connections
	// Create all transformations
	World = new Transform();
	rotateWorld = new Transform();
	rotateWorld->transform(glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	scaleLobby = new Transform();
	scaleLobby->transform(glm::scale(glm::vec3(0.45f, 0.45f, 0.45f)));
	rotateLobby = new Transform();
	rotateLobby->transform(glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	scaleUserAstronaut = new Transform();
	scaleUserAstronaut->transform(glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)));
	translateUserAstronaut = new Transform();
	translateUserAstronaut->transform(glm::translate(glm::vec3(-0.5f, 0.0f, 6.0f)));
	rotateUserAstronaut = new Transform();

	World->addChild(rotateWorld);
	rotateWorld->addChild(scaleLobby);
	rotateWorld->addChild(translateUserAstronaut);
	translateUserAstronaut->addChild(rotateUserAstronaut);
	rotateUserAstronaut->addChild(scaleUserAstronaut);

	// Create lobby object
	lobby = new Lobby("obj/among_us/amongus_lobby.obj", 1, 1, 0);
	lobby->setModelMaterialProperties(
		glm::vec3(0.50754, 0.50754, 0.50754),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.19225, 0.19225, 0.19225),
		0.1f * 128
	);
	lobby->loadTexture("textures/amongus_lobby.png");

	// Create bounding spheres for the obstacles in the lobby
	// obstacles[0] = left box, obstacles[1] = right box
	obstacles.push_back(new BoundingSphere(1.43041, glm::vec3(-4.38001, 0.839228, 5.02942)));
	obstacles.push_back(new BoundingSphere(1.43041, glm::vec3(4.73001, 0.839228, 3.52939)));

	// Creating bounding planes for the walls 
	// walls[0] = left wall, walls[1] = stairs wall, walls[2] = right wall, walls[3] = right diag wall
	// walls[4] = bottom wall, walls[5] = left diag wall
	walls.push_back(new BoundingPlane(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-7.83008, 0.839228, 2.86939)));
	walls.push_back(new BoundingPlane(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0300001, 0.839228, 1.14939)));
	walls.push_back(new BoundingPlane(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(7.73008, 0.839228, 2.75939)));
	walls.push_back(new BoundingPlane(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(6.64006, 0.839228, 8.6895)));
	walls.push_back(new BoundingPlane(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.19, 0.839228, 9.83953)));
	walls.push_back(new BoundingPlane(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(-6.81006, 0.839228, 8.6595)));

	userAstronaut = new AmongUsObject("obj/among_us/amongus_astro_still.obj", 0, 0, 1);
	userAstronaut->setModelMaterialProperties(
		glm::vec3(197.0f / 255.0f, 18.0f / 255.0f, 17.0f / 255.0f),
		glm::vec3(197.0f / 255.0f, 18.0f / 255.0f, 17.0f / 255.0f),
		glm::vec3(197.0f / 255.0f, 18.0f / 255.0f, 17.0f / 255.0f),
		0.0f * 128
	);
	// Update the bounding sphere to match the model
	((AmongUsObject*)userAstronaut)->updateBoundingSphere(glm::translate(glm::vec3(-0.5f, 0.0f, 6.0f)) *
		glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)));

	scaleLobby->addChild(lobby);
	scaleUserAstronaut->addChild(userAstronaut);

	// Add user-controlled astronaut to allAstronauts
	allAstronauts.push_back(userAstronaut);
	allAstroTranslates.push_back(translateUserAstronaut);
	allAstroRotates.push_back(rotateUserAstronaut);

	// Initialize all other astronauts that will be in the lobby
	initializeOtherAstronauts();

	return true;
}

void Window::initializeOtherAstronauts() {
	Geometry* astronaut;
	Transform* scaleAstronaut;
	Transform* translateAstronaut;
	Transform* rotateAstronaut;
	for (int i = 0; i < 3; i++) {
		// Create astronaut Geometry to be rendered, which own color (bounding sphere is init'd here)
		astronaut = new AmongUsObject("obj/among_us/amongus_astro_still.obj", 0, 0, 1);
		astronaut->setModelMaterialProperties(
			glm::vec3(197.0f / 255.0f, 18.0f / 255.0f, 17.0f / 255.0f),
			glm::vec3(197.0f / 255.0f, 18.0f / 255.0f, 17.0f / 255.0f),
			glm::vec3(197.0f / 255.0f, 18.0f / 255.0f, 17.0f / 255.0f),
			0.0f * 128
		);
		// Update the bounding sphere to match the model
		((AmongUsObject*)astronaut)->updateBoundingSphere(glm::translate(glm::vec3((float)i, 0.0f, 2.5f)) *
			glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)));

		// Set up initial scale and position for this astronaut
		scaleAstronaut = new Transform();
		scaleAstronaut->transform(glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)));
		translateAstronaut = new Transform();
		translateAstronaut->transform(glm::translate(glm::vec3((float)i, 0.0f, 2.5f)));
		rotateAstronaut = new Transform();

		// Attach this astronaut to the scene graph to be rendered
		rotateWorld->addChild(translateAstronaut);
		translateAstronaut->addChild(rotateAstronaut);
		rotateAstronaut->addChild(scaleAstronaut);
		scaleAstronaut->addChild(astronaut);

		// Add this astronaut to the list of astronauts
		allAstronauts.push_back(astronaut);
		allAstroTranslates.push_back(translateAstronaut);
		allAstroRotates.push_back(rotateAstronaut);
	}
}

// Material property values: http://devernay.free.fr/cours/opengl/materials.html
bool Window::initializeObjects()
{
	lightSphere = new LightSource("obj/sphere.obj", 10);
	lightSphere->setModelMaterialProperties(
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(1.0f, 0.5f, 0.31f),
		0.1f * 128
	);

	// Initialize light source properties
	lightSphere->initializeLightSourceProperties(shaderProgram, eyePos);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
	delete sphere;
	delete bunnyPoints;
	delete bearPoints;
	delete sandalPoints;
	delete cubePoints;
	delete lightSphere;

	// Deallocate among us scene graph nodes
	delete lobby;
	//delete userAstronaut;
	delete scaleLobby;
	delete rotateUserAstronaut;
	delete testSphere;

	for (int i = 0; i < obstacles.size(); i++) {
		delete obstacles[i];
	}

	for (int i = 0; i < walls.size(); i++) {
		delete walls[i];
	}

	for (int i = 0; i < allAstronauts.size(); i++) {
		delete allAstronauts[i];
		delete allAstroTranslates[i];
		delete allAstroRotates[i];
	}

	// Deallocate scene graph nodes
	delete World;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyBoxShaderProgram);
	glDeleteProgram(sphereShaderProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	if (moving != -1) {
		bool hasCollided = detectCollisions(userAstronaut);
		// The user is holding a key to move/turn the camera
		updatePlayerIfKeyHold(hasCollided);
	}

	// Move all astronauts randomly
	for (int i = 0; i < allAstronauts.size(); i++) {
		Geometry* currAstro = allAstronauts[i];
		Transform* currAstroTranslate = allAstroTranslates[i];
		// If the current astro is the user, don't randomly move the user
		if (userAstronaut == currAstro)
			continue;
		//if (detectCollisions(currAstro))
			//continue;
		currAstroTranslate->transform(glm::translate(glm::vec3(0.0f, 0.0f, 0.01f)));
		((AmongUsObject*)currAstro)->updateBoundingSphere(glm::translate(glm::vec3(0.0f, 0.0f, 0.01f)));
	}
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render among us scene graph
	World->draw(glm::mat4(1), view, projection, eyePos, shaderProgram);
	// Draw light sphere since lightSphere holds the light source that will illuminate the object
	lightSphere->draw(view, projection, shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

// if the user is colliding with something, don't let the user move in the current userDirection
bool Window::detectCollisions(Geometry* obj) {
	BoundingSphere* bSphere = ((AmongUsObject*)obj)->getBoundingSphere();

	// Check collisions with any obstacles
	for (int i = 0; i < obstacles.size(); i++) {
		if (bSphere->detectCollision(obstacles[i])) {
			//std::cout << "Collided with sphere at " << obstacles[i]->getPosition().x << ", " << obstacles[i]->getPosition().y << ", " << obstacles[i]->getPosition().z << std::endl;
			return true;
		}
	}
	// Check collisions with any walls
	for (int i = 0; i < walls.size(); i++) {
		if (bSphere->detectCollisionWithWall(walls[i])) {
			//std::cout << "Collided with wall at " << walls[i]->getPosition().x << ", " << walls[i]->getPosition().y << ", " << walls[i]->getPosition().z << std::endl;
			return true;
		}
	}

	// Check collisions with any other astronauts (that isn't this astronaut)
	for (int i = 0; i < allAstronauts.size(); i++) {
		BoundingSphere* otherSphere = ((AmongUsObject*)allAstronauts[i])->getBoundingSphere();
		if (obj != allAstronauts[i] && bSphere->detectCollision(otherSphere)) {
			return true;
		}
	}

	return false;
}

void Window::updatePlayerIfKeyHold(bool collision) {
	glm::vec3 rotateDirection;
	float rot_angle;

	// The direction the user will face
	glm::vec3 newDirection(0.0f, 0.0f, 0.0f);
	// The direction and amount that the user will move
	glm::vec3 moveDirection(0.0f, 0.0f, 0.0f);
	if (is_W_down) {
		newDirection += glm::vec3(0.0f, 0.0f, 1.0f);
		moveDirection += glm::vec3(0.0f, 0.0f, -0.01f);
	}
	if (is_A_down) {
		newDirection += glm::vec3(1.0f, 0.0f, 0.0f);
		moveDirection += glm::vec3(-0.01f, 0.0f, 0.0f);
	}
	if (is_S_down) {
		newDirection += glm::vec3(0.0f, 0.0f, -1.0f);
		moveDirection += glm::vec3(0.0f, 0.0f, 0.01f);
	}
	if (is_D_down) {
		newDirection += glm::vec3(-1.0f, 0.0f, 0.0f);
		moveDirection += glm::vec3(0.01f, 0.0f, 0.0f);
	}

	// Change the direction of the user if facing a new direction
	if (newDirection != userDirection) {
		updateAstronautDirection(newDirection, userDirection, rotateUserAstronaut);
		userDirection = newDirection;
	}
	else if (collision) {
		// If the astronaut is going in the same direction, but there's a collision, don't let the user keep moving in that direction
		return;
	}

	// Move the user and its corresponding bounding sphere
	translateUserAstronaut->transform(glm::translate(moveDirection));
	((AmongUsObject*)userAstronaut)->updateBoundingSphere(glm::translate(moveDirection));
}

void Window::updateAstronautDirection(glm::vec3 newDirection, glm::vec3 currDirection, Transform* rotateSpecificAstronaut) {
	glm::vec3 rotateDirection = newDirection - currDirection;
	float velocity = glm::length(rotateDirection);
	glm::vec3 rotAxis = glm::cross(currDirection, newDirection);
	//std::cout << "curr dir: " << currDirection.x << ", " << currDirection.y << ", " << currDirection.z << std::endl;
	//std::cout << "new dir: " << newDirection.x << ", " << newDirection.y << ", " << newDirection.z << std::endl;

	float dotProd = glm::dot(glm::normalize(newDirection), glm::normalize(currDirection));
	//std::cout << "dot prod: " << dotProd << std::endl;
	float rot_angle = glm::acos(dotProd);

	// If the rotation axis == (0,0,0), then the two vectors are antiparallel, so will flip by 180
	if (rotAxis == glm::vec3(0, 0, 0)) {
		rotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		rot_angle = glm::radians(180.0f);
	}
	//std::cout << "Angle: " << glm::degrees(rot_angle) << " degrees, " << rot_angle << " radians" << "; rotAxis: " << rotAxis.x << ", " << rotAxis.y << ", " << rotAxis.z << std::endl;
	//std::cout << "---------------------------------------------------------" << std::endl;
	rotateSpecificAstronaut->transform(glm::rotate(rot_angle, rotAxis));
}

void Window::updateCameraIfKeyHold() {
	switch (moving) {
	case 0:
		// move forward
		eyePos = glm::translate(glm::vec3(0.0f, 0.0f, -0.05f)) * glm::vec4(eyePos, 1.0f);
		lookAtPoint = glm::translate(glm::vec3(0.0f, 0.0f, -0.05f)) * glm::vec4(lookAtPoint, 1.0f);
		// Update view vector with new eyePos
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
		break;
	case 1:
		// moving backward
		eyePos = glm::translate(glm::vec3(0.0f, 0.0f, 0.05f)) * glm::vec4(eyePos, 1.0f);
		lookAtPoint = glm::translate(glm::vec3(0.0f, 0.0f, 0.05f)) * glm::vec4(lookAtPoint, 1.0f);
		// Update view vector with new eyePos
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
		break;
	case 2:
		// Move up
		eyePos = glm::translate(glm::vec3(0.0f, 0.05f, 0.0f)) * glm::vec4(eyePos, 1.0f);
		lookAtPoint = glm::translate(glm::vec3(0.0f, 0.05f, 0.0f)) * glm::vec4(lookAtPoint, 1.0f);
		// Update view vector with new eyePos
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
		break;
	case 3:
		// Move down
		// If we are at ground level, don't go down any more (don't go past ground)
		if (eyePos.y <= -5)
			break;
		eyePos = glm::translate(glm::vec3(0.0f, -0.05f, 0.0f)) * glm::vec4(eyePos, 1.0f);
		lookAtPoint = glm::translate(glm::vec3(0.0f, -0.05f, 0.0f)) * glm::vec4(lookAtPoint, 1.0f);
		// Update view vector with new eyePos
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
		break;
	case 4:
		// Turn left
		lookAtPoint = glm::translate(-eyePos) * glm::vec4(lookAtPoint, 1.0f);
		lookAtPoint = glm::rotate(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lookAtPoint, 1.0f);
		lookAtPoint = glm::translate(eyePos) * glm::vec4(lookAtPoint, 1.0f);
		// Update view vector with new lookat
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
		break;
	case 5: 
		// Turn right
		lookAtPoint = glm::translate(-eyePos) * glm::vec4(lookAtPoint, 1.0f);
		lookAtPoint = glm::rotate(glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lookAtPoint, 1.0f);
		lookAtPoint = glm::translate(eyePos) * glm::vec4(lookAtPoint, 1.0f);
		// Update view vector with new lookat
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
		break;
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	// If y < 0, then scroll down & if y > 0, then scroll up
	if (yoffset > 0) {
		// move forward
		eyePos = glm::translate(glm::vec3(0.0f, 0.0f, -0.2f)) * glm::vec4(eyePos, 1.0f);
		lookAtPoint = glm::translate(glm::vec3(0.0f, 0.0f, -0.2f)) * glm::vec4(lookAtPoint, 1.0f);
		// Update view vector with new eyePos
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
	}
	else if (yoffset < 0) {
		// moving backward
		eyePos = glm::translate(glm::vec3(0.0f, 0.0f, 0.2f)) * glm::vec4(eyePos, 1.0f);
		lookAtPoint = glm::translate(glm::vec3(0.0f, 0.0f, 0.2f)) * glm::vec4(lookAtPoint, 1.0f);
		// Update view vector with new eyePos
		view = glm::lookAt(eyePos, lookAtPoint, upVector);
	}
}

void Window::onMouseButtonDown(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			// Get position of cursor when clicked
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			glm::vec2 pos(xpos, ypos);
			// Init rotate for the world
			initRotateModel(Window::width, Window::height, pos);
			//((Lobby*)lobby)->initRotateModel(Window::width, Window::height, pos);
			//((AmongUsObject*)userAstronaut)->initRotateModel(Window::width, Window::height, pos);
		}
		else if (action == GLFW_RELEASE) {
			//((Lobby*)lobby)->endRotateModel();
			//((AmongUsObject*)userAstronaut)->endRotateModel();
			endRotateModel();
		}
	}
}

void Window::onMouseMove(GLFWwindow* window, double xpos, double ypos) {
	glm::vec2 curPos(xpos, ypos);
	//((Lobby*)lobby)->rotateModel(Window::width, Window::height, curPos);
	//((AmongUsObject*)userAstronaut)->rotateModel(Window::width, Window::height, curPos);
	std::pair<glm::vec3, float> rotateAxisAndAngle = rotateModel(Window::width, Window::height, curPos);
	if (rotateAxisAndAngle.second != 0.0f)
		rotateWorld->transform(glm::rotate(glm::radians(rotateAxisAndAngle.second), rotateAxisAndAngle.first));
}

void Window::initRotateModel(int windowWidth, int windowHeight, glm::vec2 cursorPos) {
	// Turn on user interactive rotations
	movement = 0;

	// Map the mouse position to a logical sphere location and keep track of the last known mouse position
	lastCursorPos = trackBallMapping(windowWidth, windowHeight, cursorPos);

	// Set the OpenGL state to modify the MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

std::pair<glm::vec3, float> Window::rotateModel(int windowWidth, int windowHeight, glm::vec2 currCursorPos) {
	glm::vec3 currSpherePos, rotateDirection;
	float rot_angle;

	// Check which type of movement we are doing
	if (movement == 0) {
		// Map this mouse position to it's logical sphere location
		currSpherePos = trackBallMapping(windowWidth, windowHeight, currCursorPos);

		// Determine the direction that the object should be rotated in logical sphere
		rotateDirection = currSpherePos - lastCursorPos;
		float velocity = glm::length(rotateDirection);

		// Only rotate if there is a decent amount of movement
		if (velocity > 0.0001f) {
			// Rotate about the axis that is perpendicular to the two points
			glm::vec3 rotAxis = glm::cross(lastCursorPos, currSpherePos);
			rot_angle = velocity * 8;
			return std::make_pair(rotAxis, rot_angle);
		}

		// Save the current point location for the next movement
		lastCursorPos = currSpherePos;
	}

	return std::make_pair(glm::vec3(), 0.0f);
}

void Window::endRotateModel() {
	// Turn off rotation
	movement = -1;
}

// Calculate 3D position of a projected unit vector onto the xy-plane
glm::vec3 Window::trackBallMapping(int windowWidth, int windowHeight, glm::vec2 cursorPos) {
	glm::vec3 v;
	float d;
	v.x = (2.0 * cursorPos.x - windowWidth) / windowWidth;
	v.y = (windowHeight - 2.0 * cursorPos.y) / windowHeight;
	v.z = 0.0f;
	d = glm::length(v);
	d = (d < 1.0f) ? d : 1.0f;
	v.z = sqrtf(1.001f - d * d);
	v = glm::normalize(v);
	return v;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		GLfloat sizeIncrement = 1;
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		// switch between the cube and the cube pointCloud
		case GLFW_KEY_F1:
			currObj = bearPoints;
			break;
		case GLFW_KEY_F2:
			currObj = bunnyPoints;
			break;
		case GLFW_KEY_F3:
			currObj = sandalPoints;
			break;
		// Rotate model when mouse button down
		case GLFW_KEY_1:
			rotateType = 1;
			anim1 = !anim1;
			std::cout << "Switched to rotate type " << rotateType << std::endl;
			break;
		// Rotate light source when mouse button down
		case GLFW_KEY_2:
			rotateType = 2;
			anim2 = !anim2;
			std::cout << "Switched to rotate type " << rotateType << std::endl;
			break;
		// Rotate model and light source together when mouse button down
		case GLFW_KEY_3:
			rotateType = 3;
			anim3 = !anim3;
			std::cout << "Switched to rotate type " << rotateType << std::endl;
			break;
		case GLFW_KEY_L:
			// Turn left -- translate lookAtPoint to make eyePos = it's origin, then rotate it about origin (eyePos), then
			// translate back to original position
			moving = 4;
			break;
		case GLFW_KEY_R:
			// Turn right -- translate lookAtPoint to make eyePos = it's origin, then rotate it about origin (eyePos), then
			// translate back to original position			
			moving = 5;
			break;
		case GLFW_KEY_W:
			// Move forward
			moving = 0;
			is_W_down = true;
			break;
		case GLFW_KEY_A:
			// Move left
			moving = 2;
			is_A_down = true;
			break;
		case GLFW_KEY_S:
			// Move backward
			moving = 1;
			is_S_down = true;
			break;
		case GLFW_KEY_D:
			// Move right
			moving = 3;
			is_D_down = true;
			break;
		case GLFW_KEY_N:
			// Switch rendering mode between normal coloring and Phong illumination model
			//((PointCloud*)currObj)->changeRenderingMode();
			break;
		default:
			break;
		}
	}
	if (action == GLFW_RELEASE) {
		// User has lifted key, so stop moving
		switch (key) {
		case GLFW_KEY_W:
			// Move forward
			is_W_down = false;
			break;
		case GLFW_KEY_A:
			// Move left
			is_A_down = false;
			break;
		case GLFW_KEY_S:
			// Move backward
			is_S_down = false;
			break;
		case GLFW_KEY_D:
			// Move right
			is_D_down = false;
			break;
		}
		moving = -1;
	}
}
