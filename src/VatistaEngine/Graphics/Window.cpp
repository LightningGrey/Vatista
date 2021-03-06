#include "Window.h"

namespace Vatista {
	Window::Window() : window(nullptr), width(1920), height(1080), windowName("Vatista")
	{
		init();
	}

	Window::Window(int widthV, int heightV, const char* windowNameV) : window(nullptr), width(widthV),
		height(heightV), windowName(windowNameV)
	{
		init();
	}
	
	Window::~Window()
	{
		glfwDestroyWindow(window);
	}

	bool Window::init()
	{
		//initializing GLFW
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); 
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, windowName, NULL, NULL);
		glfwMakeContextCurrent(window);
		//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		if (window == NULL) {
			VATISTA_LOG_ERROR("Window creation failure");
			glfwTerminate();
			return false;
		}
		//error if GLAD is not loaded 
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			VATISTA_LOG_ERROR("GLAD load failure");
			return false;
		}
		VATISTA_LOG_INFO("Window initialization successful.");

		return true;
	}

	void Window::clear()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//callback function
	void Window::framebuffer_size_callback(GLFWwindow*, 
		int windowWidth, int windowHeight) {
		glViewport(0, 0, windowWidth, windowHeight);
	}

	void Window::processInput(GLFWwindow* currentWindow) {
		if (glfwGetKey(currentWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(currentWindow, true);
		}
	}

	bool Window::shouldClose()
	{
		if (glfwWindowShouldClose(window)) {
			return true;
		}
		else {
			return false;
		}
	}

	//void Window::resize(int width, int height)
	//{
	//	myAspect = newWidth / (float)newHeight;
	//	myCamera->Projection = glm::perspective(glm::radians(60.0f),
	//		myAspect, 0.01f, 1000.0f);
	//}

	GLFWwindow* Window::getWindow()
	{
		return window;
	}
	int Window::getWidth()
	{
		return width;
	}
	void Window::setWidth(int newWidth)
	{
		width = newWidth;
	}
	int Window::getHeight()
	{
		return height;
	}
	void Window::setHeight(int newHeight)
	{
		height = newHeight;
	}
}