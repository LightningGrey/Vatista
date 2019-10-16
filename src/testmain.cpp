#include "Window.h"
#include "Log.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "ObjLoader.h"

int main() {
	Window test(1600, 900, "Lumen Stella");
	test.checkRun();
	test.renderLoop();

	glfwTerminate();
	return 0;
}