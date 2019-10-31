#include "Game.h"

Vatista::Game::Game() : gameWindow(nullptr), clearColour(glm::vec4(0, 0, 0, 1)),
	windowName("Vatista Engine")
{
}

Vatista::Game::Game(std::string newName) : gameWindow(nullptr),
clearColour(glm::vec4(0, 0, 0, 1)), windowName(newName)
{
}

Vatista::Game::~Game()
{
}

void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//Vatista::Window* gameWindow = ;
	//if (gameWindow != nullptr) {
	//	gameWindow->getWindow()->resize(width, height);
	//}
}

void Vatista::Game::run()
{
	init();

	static float previousFrame = glfwGetTime();

	while (!gameWindow->shouldClose()) {
		static float currentFrame = glfwGetTime();
		static float deltaTime = currentFrame - previousFrame;

		update(deltaTime);
		draw(deltaTime);

		glfwSwapBuffers(gameWindow->getWindow());
		glfwPollEvents();

		previousFrame = currentFrame;
	}

	close();
}

void Vatista::Game::init()
{
	gameWindow = new Vatista::Window(1600, 900, "Alpha Strike");

	myCamera = std::make_shared<Vatista::Camera>();
	myCamera->SetPosition(glm::vec3(0, 5, -6));
	myCamera->LookAt(glm::vec3(0));
	myCamera->Projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.01f, 1000.0f);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;
	std::vector<uint32_t> indices;

	bool objectLoad = loader.load("./obj/x64/Alpha Strike/testsword.obj", vertices, UVs, normals, indices);

	if (objectLoad) {
		myMesh = std::make_shared<Mesh>(vertices, vertices.size(), UVs,
			UVs.size(), normals, normals.size(), indices, indices.size());
		meshList.push_back(myMesh);
	}

	myShader = std::make_shared<Shader>();
	myShader->Load("./obj/x64/Vatista/passthrough.vs", 
		"./obj/x64/Vatista/passthrough.fs");

	modelTransform = glm::mat4(1.0f);
	modelTransform = glm::translate(modelTransform, glm::vec3(0.0f, 0.0f, 0.2f));
}

void Vatista::Game::close()
{
	glfwTerminate();
}


void Vatista::Game::update(float dt)
{
	glm::vec3 movement = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	float speed = 1.0f;
	float rotSpeed = 1.0f;

	//movement and rotation of the camera
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		movement.z -= speed * dt;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		movement.z += speed * dt;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		movement.x -= speed * dt;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		movement.x += speed * dt;

	// Rotate and move our camera based on input
	myCamera->Rotate(rotation);
	myCamera->Move(movement);
}

void Vatista::Game::draw(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	myShader->Bind();

	//draw 
	myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * modelTransform);
	for (int i = 0; i < meshList.size(); i++) {
		meshList[i]->Draw();
	}
}
