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

// Objects to Render
Cube * Window::cube;
Sphere* Window::sphere;
PointCloud * Window::cubePoints;
PointCloud* Window::bunnyPoints;
PointCloud* Window::sandalPoints;
PointCloud* Window::bearPoints;

// Among us geometries and transforms
Geometry* Window::lobby;
Geometry* Window::userAstronaut;
Geometry* Window::testSphere;
Transform* Window::transformSphere;
Transform* Window::rotateWorld;
Transform* Window::rotateLobby;
Transform* Window::scaleLobby;
Transform* Window::translateAstronaut;
Transform* Window::rotateAstronaut;
Transform* Window::scaleAstronaut;

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
	// Left obstacle: pos = (6.00624,2.30415,-1.10025), r = 1.43041
	testSphere = new AmongUsObject("obj/texsphere.obj", 0, 0, 0);
	testSphere->setModelMaterialProperties(
		glm::vec3(0.50754, 0.50754, 0.50754),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.19225, 0.19225, 0.19225),
		0.1f * 128
	);
	transformSphere = new Transform();
	transformSphere->transform(glm::translate(glm::vec3(-16.5f, -9.7f, 2.8f)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)));
	transformSphere->addChild(testSphere);

	World = new Transform();
	rotateWorld = new Transform();
	rotateWorld->transform(glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	scaleLobby = new Transform();
	scaleLobby->transform(glm::scale(glm::vec3(0.45f, 0.45f, 0.45f)));
	rotateLobby = new Transform();
	rotateLobby->transform(glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	scaleAstronaut = new Transform();
	scaleAstronaut->transform(glm::scale(glm::vec3(0.9f, 0.9f, 0.9f)));
	translateAstronaut = new Transform();
	translateAstronaut->transform(glm::translate(glm::vec3(0.0f, 0.0f, 5.0f)));
	rotateAstronaut = new Transform();
	rotateAstronaut->transform(glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	//World->addChild(rotateLobby);
	//rotateLobby->addChild(scaleLobby);
	World->addChild(rotateWorld);
	rotateWorld->addChild(scaleLobby);
	rotateWorld->addChild(translateAstronaut);
	translateAstronaut->addChild(scaleAstronaut);
	//rotateAstronaut->addChild(scaleAstronaut);

	// Create lobby object
	lobby = new Lobby("obj/among_us/amongus_lobby.obj", 1, 1, 0);
	lobby->setModelMaterialProperties(
		glm::vec3(0.50754, 0.50754, 0.50754),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.19225, 0.19225, 0.19225),
		0.1f * 128
	);
	lobby->loadTexture("textures/amongus_lobby.png");

	userAstronaut = new AmongUsObject("obj/among_us/amongus_astro_still.obj", 0, 0, 1);
	userAstronaut->setModelMaterialProperties(
		glm::vec3(197.0f / 255.0f, 18.0f / 255.0f, 17.0f / 255.0f),
		glm::vec3(197.0f / 255.0f, 18.0f / 255.0f, 17.0f / 255.0f),
		glm::vec3(197.0f / 255.0f, 18.0f / 255.0f, 17.0f / 255.0f),
		0.0f * 128
	);

	scaleLobby->addChild(lobby);
	scaleAstronaut->addChild(userAstronaut);
	return true;
}

// Material property values: http://devernay.free.fr/cours/opengl/materials.html
bool Window::initializeObjects()
{
	// Create cubemap as our skybox
	// cube = new Cube(&cubemapTextureID);

	// Create tesselated sphere
	//sphere = new Sphere();

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
	delete userAstronaut;
	delete scaleLobby;
	delete scaleAstronaut;
	delete testSphere;

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
	/* Updates to scene graph to animate the ride 
	if (anim1) {
		if (upOrDown == -1) {
			translateWheelUpAndDown->transform(glm::translate(glm::vec3(0.0f, -0.01f, 0.0f)));
			counter -= 0.01f;
			// Wheel has reached min point, so now go back down
			if (counter <= 5.0f)
				upOrDown = 1;
		}
		else if (upOrDown == 1) {
			translateWheelUpAndDown->transform(glm::translate(glm::vec3(0.0f, 0.01f, 0.0f)));
			counter += 0.01f;
			// Wheel has reached max point, so now go back down
			if (counter >= 13.5f) {
				upOrDown = -1;
			}
		}
	}
	if (anim2)
		rotateWheel->transform(glm::rotate(glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f)));
	if (anim3) {
		for (int i = 0; i < rotateCars.size(); i++)
			rotateCars[i]->transform(glm::rotate(glm::radians(-0.3f), glm::vec3(0.0f, 1.0f, 0.0f)));
	}
	*/

	if (moving != -1) {
		// The user is holding a key to move/turn the camera
		updateCameraIfKeyHold();
	}
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render among us scene graph
	World->draw(glm::mat4(1), view, projection, eyePos, shaderProgram);
	transformSphere->draw(glm::mat4(1), view, projection, eyePos, shaderProgram);
	// Draw light sphere since lightSphere holds the light source that will illuminate the object
	lightSphere->draw(view, projection, shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
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
	/* If rotateType = 1, scale model; if 2, change distance of light from center; if 3, do both 1 and 2
	if (rotateType == 1 || rotateType == 3)
		((PointCloud*)currObj)->updateModelSize(yoffset);
	if (rotateType == 2 || rotateType == 3)
		lightSphere->updateLightPositionToCenter(yoffset);
	*/
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
			// Increase point size
			//((PointCloud*)currObj)->updatePointSize(sizeIncrement);
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
			break;
		case GLFW_KEY_A:
			// Move up
			moving = 2;
			break;
		case GLFW_KEY_S:
			// Move backward
			moving = 1;
			break;
		case GLFW_KEY_D:
			// Move down
			moving = 3;
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
		moving = -1;
	}
}
