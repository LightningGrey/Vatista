#include "Window.h"

Window::Window() : width(1920), height(1080), windowName("Vatista")
{
	//initializing GLFW
	glfwInit();

	window = glfwCreateWindow(width, height, windowName, NULL, NULL);
	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Window::Window(int widthV, int heightV, const char* windowNameV) : width(widthV),
	height(heightV), windowName(windowNameV)
{
	//initializing GLFW
	glfwInit();

	window = glfwCreateWindow(width, height, windowName, NULL, NULL);
	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

void Window::init()
{
}

float Window::checkRun()
{
	if (window == NULL) {
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1.0f;
	}
	//error if GLAD is not loaded 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GLAD." << std::endl;
		return -1.0f;
	}
	GAME_LOG_INFO("Window initialization successful.");
	return 0;
}

void Window::clear()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::renderLoop()
{
	while (!glfwWindowShouldClose(window))
	{

		clear();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//callback function
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Window::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
