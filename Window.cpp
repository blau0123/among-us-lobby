#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

// Objects to Render
Cube * Window::cube;
Sphere* Window::sphere;
PointCloud * Window::cubePoints;
PointCloud* Window::bunnyPoints;
PointCloud* Window::sandalPoints;
PointCloud* Window::bearPoints;
LightSource* Window::lightSphere;
Object* currObj;

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
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeSceneGraph() {
	// Set up scene graph and connections
	World = new Transform();
}

// Material property values: http://devernay.free.fr/cours/opengl/materials.html
bool Window::initializeObjects()
{
	// Create cubemap as our skybox
	cube = new Cube(5.0f);

	// Create tesselated sphere
	sphere = new Sphere();
	sphere->init("obj/trisphere.obj");

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

	lightSphere = new LightSource("obj/sphere.obj", 10);
	lightSphere->setModelMaterialProperties(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		// Color of the light
		glm::vec3(1.0f, 0.5f, 0.31f),
		0.1f
	);

	// Initialize light source properties
	lightSphere->initializeLightSourceProperties(shaderProgram, eyePos);

	// Set the bear point cloud to be the first thing to show
	currObj = bearPoints;

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
	currObj->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	sphere->draw(view, projection, eyePos, sphereShaderProgram);
	// Always render skybox last
	cube->draw(view, projection, skyBoxShaderProgram);

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
}

void Window::onMouseMove(GLFWwindow* window, double xpos, double ypos) {
	glm::vec2 curPos(xpos, ypos);
	if (rotateType == 1 || rotateType == 3)
		((PointCloud*)currObj)->rotateModel(Window::width, Window::height, curPos);
	if (rotateType == 2 || rotateType == 3)
		lightSphere->rotateModel(Window::width, Window::height, curPos);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
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
			std::cout << "Switched to rotate type " << rotateType << std::endl;
			break;
		// Rotate light source when mouse button down
		case GLFW_KEY_2:
			rotateType = 2;
			std::cout << "Switched to rotate type " << rotateType << std::endl;
			break;
		// Rotate model and light source together when mouse button down
		case GLFW_KEY_3:
			rotateType = 3;
			std::cout << "Switched to rotate type " << rotateType << std::endl;
			break;
		case GLFW_KEY_L:
			// Increase point size
			((PointCloud*)currObj)->updatePointSize(sizeIncrement);
			break;
		case GLFW_KEY_S:
			// Decrease point size
			((PointCloud*)currObj)->updatePointSize(-sizeIncrement);
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
