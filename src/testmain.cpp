#include "Window.h"
#include "Log.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "ObjLoader.h"

#include "GLM/gtc/matrix_transform.hpp";

int main() {
	// Stores the main window that the game is running in
	GLFWwindow* myWindow;
	// Stores the clear color of the game's window
	glm::vec4   myClearColor;
	// Stores the title of the game's window
	char        myWindowTitle[32];

	// A shared pointer to our mesh
	Vatista::Mesh_sptr   myMesh;
	// A shared pointer to our shader
	Vatista::Shader_sptr myShader;

	Vatista::Camera::Sptr myCamera;

	glm::mat4 myModelTransform;
	glm::mat4 myModelTransform2;

	Vatista::ObjLoader loader;

	myCamera = std::make_shared<Vatista::Camera>();
	myCamera->SetPosition(glm::vec3(7, 5, 0));
	myCamera->LookAt(glm::vec3(0));
	myCamera->Projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f);

	Vatista::Log::Init();
	VATISTA_LOG_INFO("Vatista.dll running.");
	GAME_LOG_WARN("Game only displays window.");
	GAME_LOG_ERROR("Mistake made.");
	VATISTA_LOG_TRACE("Void extermination complete.");

	Vatista::Window test(1600, 900, "Lumen Stella");
	//test.checkRun();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;
	std::vector<uint32_t> indices;

	bool loaded = loader.load("D:/Year 2 Semester 1/GDW2/Vatista/obj/x64/Vatista/monkey.obj", vertices, UVs, normals, indices);


	test.update();

	glfwTerminate();
	Vatista::Log::Uninitialize();

	return 0;
}