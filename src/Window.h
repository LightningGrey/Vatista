#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
	Window();
	Window(int width, int height, const char* windowName);
	~Window();
	void init();
	float checkRun();
	void renderLoop();
	void clear();
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);

private:
	GLFWwindow* window;
	int width, height;
	const char* windowName;
};