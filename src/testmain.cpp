#include "Window.h"

int main() {
	Window test(800, 600, "Lumen Stella");
	test.checkRun();
	test.renderLoop();

	glfwTerminate();
	return 0;
}