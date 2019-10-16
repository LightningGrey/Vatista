#include "Window.h"
#include "Log.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "ObjLoader.h"

int main() {
	Vatista::Log::Init();
	VATISTA_LOG_INFO("Vatista.dll running.");
	GAME_LOG_WARN("Game only displays window.");
	GAME_LOG_ERROR("Mistake made.");
	VATISTA_LOG_TRACE("Void extermination complete.");

	Window test(1600, 900, "Lumen Stella");
	test.checkRun();
	test.renderLoop();

	glfwTerminate();
	return 0;
}