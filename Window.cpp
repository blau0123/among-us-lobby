#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

// Animation controls (0 = no animation, 1 = animation)
int Window::anim1 = 1;
int Window::anim2 = 1;
int Window::anim3 = 1;

float Window::counter = 14.0f;
int Window::upOrDown = -1;

// Objects to Render
Cube * Window::cube;
Sphere* Window::sphere;
PointCloud * Window::cubePoints;
PointCloud* Window::bunnyPoints;
PointCloud* Window::sandalPoints;
PointCloud* Window::bearPoints;
LightSource* Window::lightSphere;
Object* currObj;

unsigned int Window::cubemapTextureID;

// Scene Graph nodes
Transform* Window::World;
Transform* Window::translateGround;
Transform* Window::translateGroundBack;
Transform* Window::translatePole;
Transform* Window::scalePole;
Transform* Window::translateWheelUpAndDown;
Transform* Window::translateWheel;
Transform* Window::scaleWheel;
Transform* Window::rotateWheel;
Transform* Window::rotateSupportPoleX;
Transform* Window::rotateSupportPoleZ;
Transform* Window::scaleAttachPole;
Transform* Window::scaleSupportPoleLeft;
Transform* Window::scaleSupportPoleRight;
Transform* Window::scaleSupportPoleFront;
Transform* Window::scaleSupportPoleBack;
std::vector<Transform*> Window::translateCars;
std::vector<Transform*> Window::rotateCars;

Geometry* Window::ground;
Geometry* Window::pole;
Geometry* Window::wheel;
Geometry* Window::car;
Geometry* Window::car2;
std::vector<Geometry*> Window::cars;
std::vector<Geometry*> Window::attachPoles;
std::vector<Geometry*> Window::supportPoles;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// Scale matrix:
glm::mat4 Window::scale;

// View Matrix:
glm::vec3 Window::eyePos(0, 5, 20);			// Camera position.
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
	if (!shaderProgram)
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
	translateGround = new Transform();
	translateGroundBack = new Transform();
	translatePole = new Transform();
	scalePole = new Transform();
	translateWheelUpAndDown = new Transform();
	translateWheel = new Transform();
	scaleWheel = new Transform();
	rotateWheel = new Transform();
	scaleSupportPoleLeft = new Transform();
	scaleSupportPoleRight = new Transform();
	scaleSupportPoleFront = new Transform();
	scaleSupportPoleBack = new Transform();
	rotateSupportPoleX = new Transform();
	rotateSupportPoleZ = new Transform();
	//translateCar = new Transform();
	scaleAttachPole = new Transform();

	World->addChild(translateGround);
	translateGround->addChild(translatePole);
	translateGround->addChild(translateGroundBack);
	translatePole->addChild(scalePole);
	//translatePole->addChild(translateWheel);
	translatePole->addChild(rotateWheel);
	//rotateWheel->addChild(translateWheel);
	rotateWheel->addChild(translateWheelUpAndDown);
	translateWheelUpAndDown->addChild(translateWheel);
	translateWheel->addChild(scaleWheel);
	// scaleWheel->addChild(rotateWheel);
	translateWheel->addChild(scaleSupportPoleLeft);
	translateWheel->addChild(scaleSupportPoleRight);
	translateWheel->addChild(scaleSupportPoleFront);
	translateWheel->addChild(scaleSupportPoleBack);
	scaleSupportPoleLeft->addChild(rotateSupportPoleX);
	scaleSupportPoleRight->addChild(rotateSupportPoleX);
	scaleSupportPoleFront->addChild(rotateSupportPoleZ);
	scaleSupportPoleBack->addChild(rotateSupportPoleZ);

	// Create all the Geometries
	ground = new Geometry("obj/cube.obj");
	ground->setModelMaterialProperties(
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(1.0f, 0.5f, 0.31f),
		0.088
	);
	pole = new Geometry("obj/cylinder.obj");
	pole->setModelMaterialProperties(
		glm::vec3(0.75164, 0.60648, 0.22648),
		glm::vec3(0, 0, 0),
		glm::vec3(0.0, 0.7, 0.0),
		0.4f
	);
	wheel = new Geometry("obj/torus_lr.obj");
	wheel->setModelMaterialProperties(
		glm::vec3(0.61424, 0.04136, 0.04136),
		glm::vec3(0.727811, 0.626959, 0.626959),
		glm::vec3(1.0f, 0.5f, 0.31f),
		0.1f
	);

	translateGroundBack->addChild(ground);
	scalePole->addChild(pole);
	scaleWheel->addChild(wheel);
	//rotateWheel->addChild(wheel);

	// Create 5 cars that will be on the outskirt of the wheel
	createRideCars();
	// Create poles that will attach each car to the torus (wheel)
	createAttachPoles();
	// Create the support poles in between pole and wheel
	createSupportPoles();

	// Add transforms to each Transformation node
	translateGround->transform(glm::translate(glm::vec3(0.0f, -7.0f, 0.0f)));
	translateGroundBack->transform(glm::translate(glm::vec3(0.0f, 0.0f, -7.0f)));
	scalePole->transform(glm::scale(glm::vec3(1.0f, 7.0f, 1.0f)));
	translateWheel->transform(glm::translate(glm::vec3(0.0f, 14.0f, 0.0f)));
	//translateWheel->transform(glm::translate(glm::vec3(0.0f, 5.0f, 0.0f)));
	scaleWheel->transform(glm::scale(glm::vec3(6.0f, 1.7f, 6.0f)));
	rotateSupportPoleX->transform(glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	rotateSupportPoleZ->transform(glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	scaleAttachPole->transform(glm::scale(glm::vec3(0.15f, 2.0f, 0.15f)));

	return true;
}

bool Window::createSupportPoles() {
	Geometry* supportPoleLeft = new Geometry("obj/cylinder.obj");
	supportPoleLeft->setModelMaterialProperties(
		// diffuse, specular, ambient
		glm::vec3(0.50754, 0.50754, 0.50754),
		glm::vec3(0.508273, 0.508273, 0.508273),
		glm::vec3(0.19225, 0.19225, 0.19225),
		0.4f
	);
	Geometry* supportPoleRight = new Geometry("obj/cylinder.obj");
	supportPoleRight->setModelMaterialProperties(
		// diffuse, specular, ambient
		glm::vec3(0.50754, 0.50754, 0.50754),
		glm::vec3(0.508273, 0.508273, 0.508273),
		glm::vec3(0.19225, 0.19225, 0.19225),
		0.4f
	);

	scaleSupportPoleLeft->transform(glm::scale(glm::vec3(3.0f, 0.3f, 0.3f)));
	scaleSupportPoleRight->transform(glm::scale(glm::vec3(-3.0f, 0.3f, 0.3f)));

	rotateSupportPoleX->addChild(supportPoleLeft);
	rotateSupportPoleX->addChild(supportPoleRight);

	Geometry* supportPoleFront = new Geometry("obj/cylinder.obj");
	supportPoleFront->setModelMaterialProperties(
		// diffuse, specular, ambient
		glm::vec3(0.50754, 0.50754, 0.50754),
		glm::vec3(0.508273, 0.508273, 0.508273),
		glm::vec3(0.19225, 0.19225, 0.19225),
		0.4f
	);
	Geometry* supportPoleBack = new Geometry("obj/cylinder.obj");
	supportPoleBack->setModelMaterialProperties(
		// diffuse, specular, ambient
		glm::vec3(0.50754, 0.50754, 0.50754),
		glm::vec3(0.508273, 0.508273, 0.508273),
		glm::vec3(0.19225, 0.19225, 0.19225),
		0.4f
	);
	rotateSupportPoleZ->addChild(supportPoleFront);
	rotateSupportPoleZ->addChild(supportPoleBack);
	scaleSupportPoleFront->transform(glm::scale(glm::vec3(0.3f, 0.3f, 3.0f)));
	scaleSupportPoleBack->transform(glm::scale(glm::vec3(0.3f, 0.3f, -3.0f)));

	supportPoles.push_back(supportPoleLeft);
	supportPoles.push_back(supportPoleRight);
	supportPoles.push_back(supportPoleFront);
	supportPoles.push_back(supportPoleBack);

	return true;
}

bool Window::createRideCars() {
	Geometry* car = new Geometry("obj/bunny.obj");
	car->setModelMaterialProperties(
		glm::vec3(0.75164, 0.60648, 0.22648),
		glm::vec3(0, 0, 0),
		glm::vec3(0.0, 0.7, 0.0),
		0.4f
	);
	Transform* translateCar = new Transform();
	Transform* rotateCar = new Transform();
	translateWheel->addChild(translateCar);
	translateCar->addChild(rotateCar);
	rotateCar->addChild(car);
	translateCar->transform(glm::translate(glm::vec3(6.0f, -4.0f, 0.0f)));
	translateCars.push_back(translateCar);
	rotateCars.push_back(rotateCar);
	cars.push_back(car);

	car = new Geometry("obj/bunny.obj");
	car->setModelMaterialProperties(
		glm::vec3(0.75164, 0.60648, 0.22648),
		glm::vec3(0, 0, 0),
		glm::vec3(0.0, 0.7, 0.0),
		0.4f
	);
	translateCar = new Transform();
	rotateCar = new Transform();
	translateWheel->addChild(translateCar);
	translateCar->addChild(rotateCar);
	rotateCar->addChild(car);
	translateCar->transform(glm::translate(glm::vec3(0.0f, -4.0f, 6.0f)));
	translateCars.push_back(translateCar);
	rotateCars.push_back(rotateCar);
	cars.push_back(car);

	car = new Geometry("obj/bunny.obj");
	car->setModelMaterialProperties(
		glm::vec3(0.75164, 0.60648, 0.22648),
		glm::vec3(0, 0, 0),
		glm::vec3(0.0, 0.7, 0.0),
		0.4f
	);
	translateCar = new Transform();
	rotateCar = new Transform();
	translateWheel->addChild(translateCar);
	translateCar->addChild(rotateCar);
	rotateCar->addChild(car);
	translateCar->transform(glm::translate(glm::vec3(-6.0f, -4.0f, 0.0f)));
	translateCars.push_back(translateCar);
	rotateCars.push_back(rotateCar);
	cars.push_back(car);

	car = new Geometry("obj/bunny.obj");
	car->setModelMaterialProperties(
		glm::vec3(0.75164, 0.60648, 0.22648),
		glm::vec3(0, 0, 0),
		glm::vec3(0.0, 0.7, 0.0),
		0.4f
	);
	translateCar = new Transform();
	rotateCar = new Transform();
	translateWheel->addChild(translateCar);
	translateCar->addChild(rotateCar);
	rotateCar->addChild(car);
	translateCar->transform(glm::translate(glm::vec3(0.0f, -4.0f, -6.0f)));
	translateCars.push_back(translateCar);
	rotateCars.push_back(rotateCar);
	cars.push_back(car);

	return true;
}

bool Window::createAttachPoles() {
	for (int i = 0; i < translateCars.size(); i++) {
		Geometry* attachPole = new Geometry("obj/cylinder.obj");
		attachPole->setModelMaterialProperties(
			// diffuse, specular, ambient
			glm::vec3(0.50754, 0.50754, 0.50754),
			glm::vec3(0.508273, 0.508273, 0.508273),
			glm::vec3(0.19225, 0.19225, 0.19225),
			0.4f
		);
		scaleAttachPole->addChild(attachPole);
		translateCars[i]->addChild(scaleAttachPole);
		attachPoles.push_back(attachPole);
	}

	return true;
}

// Material property values: http://devernay.free.fr/cours/opengl/materials.html
bool Window::initializeObjects()
{
	// Create cubemap as our skybox
	cube = new Cube(&cubemapTextureID);

	// Create tesselated sphere
	sphere = new Sphere();
	sphere->init();

	lightSphere = new LightSource("obj/sphere.obj", 10);
	lightSphere->setModelMaterialProperties(
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(1.0f, 0.5f, 0.31f),
		0.1f
	);

	// Initialize light source properties
	lightSphere->initializeLightSourceProperties(shaderProgram, eyePos);

	/*
	// Create a point cloud consisting of cube vertices.
	bunnyPoints = new PointCloud("obj/bunny.obj", 10);
	// Set the material properties for the bunny (k_d, k_s, k_a, shininess)
	bunnyPoints->setModelMaterialProperties(
		// No diffuse reflection
		glm::vec3(0, 0, 0),
		glm::vec3(0.296648, 0.296648, 0.296648),
		glm::vec3(0.0, 0.0, 0.8),
		0.088
	);
	bearPoints = new PointCloud("obj/bear.obj", 10);
	// Set the material properties for the bear (k_d, k_s, k_a, shininess)
	bearPoints->setModelMaterialProperties(
		glm::vec3(0.75164, 0.60648, 0.22648),
		// No specular component
		glm::vec3(0, 0, 0),
		glm::vec3(0.0, 0.7, 0.0),
		0.4f
	);
	sandalPoints = new PointCloud("obj/SandalF20.obj", 10);
	// Set the material properties for the sandal (k_d, k_s, k_a, shininess)
	sandalPoints->setModelMaterialProperties(
		glm::vec3(0.61424, 0.04136, 0.04136),
		glm::vec3(0.727811, 0.626959, 0.626959),
		glm::vec3(1.0f, 0.5f, 0.31f),
		0.1f
	);

	// Set the bear point cloud to be the first thing to show
	currObj = bearPoints;
	*/
	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
	delete bunnyPoints;
	delete bearPoints;
	delete sandalPoints;
	delete cubePoints;
	delete lightSphere;

	// Deallocate scene graph nodes
	delete World;
	delete translateGround;
	delete translateGroundBack;
	delete translatePole;
	delete scalePole;
	delete translateWheelUpAndDown;
	delete translateWheel;
	delete scaleWheel;
	delete rotateWheel;
	delete rotateSupportPoleX;
	delete rotateSupportPoleZ;
	delete scaleSupportPoleLeft;
	delete scaleSupportPoleRight;
	delete scaleSupportPoleFront;
	delete scaleSupportPoleBack;
	delete scaleAttachPole;

	delete ground;
	delete pole;
	delete wheel;
	delete car;
	delete car2;

	for (int i = 0; i < translateCars.size(); i++) {
		delete rotateCars[i];
		delete translateCars[i];
		delete cars[i];
		delete attachPoles[i];
	}

	for (int i = 0; i < supportPoles.size(); i++) {
		delete supportPoles[i];
	}

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
	// Perform any necessary updates here 
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
			if (counter >= 14.0f) {
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
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cube->draw(view, projection, skyBoxShaderProgram);
	// Use Phong illumination shader for Scene Graph
	World->draw(glm::mat4(1), view, projection, shaderProgram);
	lightSphere->draw(view, projection, shaderProgram);
	//sphere->draw(view, projection, eyePos, cubemapTextureID, sphereShaderProgram);
	/* Render the objects
	currObj->draw(view, projection, shaderProgram);
	// If the current render mode is 1, show the light source and if not (normal shading), don't show
	if (((PointCloud*)currObj)->getRenderMode() == 1)
		lightSphere->draw(view, projection, shaderProgram);
	*/

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	// If y < 0, then scroll down & if y > 0, then scroll up
	// If rotateType = 1, scale model; if 2, change distance of light from center; if 3, do both 1 and 2
	if (rotateType == 1 || rotateType == 3)
		((PointCloud*)currObj)->updateModelSize(yoffset);
	if (rotateType == 2 || rotateType == 3)
		lightSphere->updateLightPositionToCenter(yoffset);
}

void Window::onMouseButtonDown(GLFWwindow* window, int button, int action, int mods) {
	/*
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			// Get position of cursor when clicked
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			glm::vec2 pos(xpos, ypos);

			// If rotateType = 1, rotate model; if 2, rotate light source; if 3, rotate both
			if (rotateType == 1 || rotateType == 3)
				((PointCloud*)currObj)->initRotateModel(Window::width, Window::height, pos);
			if (rotateType == 2 || rotateType == 3)
				lightSphere->initRotateModel(Window::width, Window::height, pos);
		}
		else if (action == GLFW_RELEASE) {
			// When release the mouse click, stop rotating
			if (rotateType == 1 || rotateType == 3)
				((PointCloud*)currObj)->endRotateModel();
			if (rotateType == 2 || rotateType == 3)
				lightSphere->endRotateModel();
		}
	}
	*/
}

void Window::onMouseMove(GLFWwindow* window, double xpos, double ypos) {
	glm::vec2 curPos(xpos, ypos);
	/*
	if (rotateType == 1 || rotateType == 3)
		((PointCloud*)currObj)->rotateModel(Window::width, Window::height, curPos);
	if (rotateType == 2 || rotateType == 3)
		lightSphere->rotateModel(Window::width, Window::height, curPos);
	*/
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
			lookAtPoint = glm::translate(-eyePos) * glm::vec4(lookAtPoint, 1.0f);
			lookAtPoint = glm::rotate(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lookAtPoint, 1.0f);
			lookAtPoint = glm::translate(eyePos) * glm::vec4(lookAtPoint, 1.0f);
			// Update view vector with new lookat
			view = glm::lookAt(eyePos, lookAtPoint, upVector);
			break;
		case GLFW_KEY_R:
			// Turn right -- translate lookAtPoint to make eyePos = it's origin, then rotate it about origin (eyePos), then
			// translate back to original position			
			lookAtPoint = glm::translate(-eyePos) * glm::vec4(lookAtPoint, 1.0f);
			lookAtPoint = glm::rotate(glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lookAtPoint, 1.0f);
			lookAtPoint = glm::translate(eyePos) * glm::vec4(lookAtPoint, 1.0f);
			// Update view vector with new lookat
			view = glm::lookAt(eyePos, lookAtPoint, upVector);
			break;
		case GLFW_KEY_W:
			// Move forward
			eyePos = glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)) * glm::vec4(eyePos, 1.0f);
			lookAtPoint = glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)) * glm::vec4(lookAtPoint, 1.0f);
			// Update view vector with new eyePos
			view = glm::lookAt(eyePos, lookAtPoint, upVector);
			break;
		case GLFW_KEY_A:
			// Move up
			eyePos = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(eyePos, 1.0f);
			lookAtPoint = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lookAtPoint, 1.0f);
			// Update view vector with new eyePos
			view = glm::lookAt(eyePos, lookAtPoint, upVector);
			break;
		case GLFW_KEY_S:
			// Move backward
			eyePos = glm::translate(glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(eyePos, 1.0f);
			lookAtPoint = glm::translate(glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(lookAtPoint, 1.0f);
			// Update view vector with new eyePos
			view = glm::lookAt(eyePos, lookAtPoint, upVector);
			break;
		case GLFW_KEY_D:
			// Move down
			// If we are at ground level, don't go down any more (don't go past ground)
			if (eyePos.y <= 0)
				break;
			eyePos = glm::translate(glm::vec3(0.0f, -1.0f, 0.0f)) * glm::vec4(eyePos, 1.0f);
			lookAtPoint = glm::translate(glm::vec3(0.0f, -1.0f, 0.0f)) * glm::vec4(lookAtPoint, 1.0f);
			// Update view vector with new eyePos
			view = glm::lookAt(eyePos, lookAtPoint, upVector);
			break;
		case GLFW_KEY_N:
			// Switch rendering mode between normal coloring and Phong illumination model
			((PointCloud*)currObj)->changeRenderingMode();
			break;
		default:
			break;
		}
	}
}
