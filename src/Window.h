#pragma once

#include "Log.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Vatista {
	class Window {
	public:
		Window();
		Window(int width, int height, const char* windowName);
		~Window();
		bool init();
		void clear();
		void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void processInput(GLFWwindow* window);
		//void resize(int width, int height);
		bool shouldClose();

		GLFWwindow* getWindow();

	private:
		GLFWwindow* window;
		int width, height;
		const char* windowName;
	};
}