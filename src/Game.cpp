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
	gameWindow = new Vatista::Window(1000, 1000, "Alpha Strike");

	myCamera = std::make_shared<Vatista::Camera>();
	myCamera->SetPosition(glm::vec3(1, 0, 0));
	myCamera->LookAt(glm::vec3(0), glm::vec3(0, 1, 0));
	myCamera->Projection = glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, 0.f, 1000.0f);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;
	std::vector<uint32_t> indices;
	std::vector<Vertex> vertData;

	bool objectLoad = loader.load("./res/plane.obj", vertices, UVs, normals, indices, vertData);

	//for (int i = 0; i < indices.size(); i++)
	//	std::cout << indices.at(i) << ' ';
	//for (int i = 0; i < vertData.size(); i++) {
	//	std::cout << vertData.at(i).Position.x << vertData.at(i).Position.y << vertData.at(i).Position.z << ' ';
	//	std::cout << vertData.at(i).UV.x << vertData.at(i).UV.y << ' ';
	//	std::cout << vertData.at(i).Normal.x << vertData.at(i).Normal.y << vertData.at(i).Normal.z << ' ';
	//	std::cout << std::endl;
	//}

	if (objectLoad) {
		myMesh = std::make_shared<Mesh>(vertices, vertices.size(), UVs,
			UVs.size(), normals, normals.size(), indices, indices.size(), 
			vertData, vertData.size());
		meshList.push_back(myMesh);
	}

	texture = std::make_shared<Texture>();
	texture->loadFile("./res/Hammer.001texture.png");
	
	myShader = std::make_shared<Shader>();
	myShader->Load("./res/passthrough.vs", 
		"./res/passthrough.fs");

	modelTransform = glm::mat4(1.0f);
	modelTransform = glm::rotate(modelTransform, 3.14f, glm::vec3(0, 1, 0));
	pos1 = glm::vec3(0, 0, -1.0f);
	modelTransform = glm::translate(modelTransform, pos1);

	modelTransform2 = glm::mat4(1.0f);
	pos2 = glm::vec3(0, 0, 0);
	modelTransform2 = glm::translate(modelTransform2, pos2);
}

void Vatista::Game::close()
{
	glfwTerminate();
}


void Vatista::Game::update(float dt)
{
	glm::vec3 movement = glm::vec3(0.0f);
	glm::vec3 movement2 = glm::vec3(0.0f);
	//glm::vec3 rotation = glm::vec3(0.0f);
	float speed = 1.0f;
	float rotSpeed = 1.0f;

	//movement and rotation of the camera
	//if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
	//	//movement.z -= speed * dt;
	//	movement.x -= speed * 0.001f;
	//if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
	//	//movement.x += speed * dt;
	//	movement.x -= speed * 0.001f;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		movement.z -= speed * 0.001f;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		movement.z += speed * 0.002f;

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
		movement2.z += speed * 0.002f;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		movement2.z -= speed * 0.001f;


	pos1 += movement;
	if (pos1.z > 5.f)
	{
		pos1.z = 5.f;
		movement.z = 0;
	}
	if (pos1.z < -5.f)
	{
		pos1.z = -5.f;
		movement.z = 0;
	}
	pos2 += movement2;
	if (pos2.z > 5.f)
	{
		pos2.z = 5.f;
		movement2.z = 0;
	}
	if (pos2.z < -5.f)
	{
		pos2.z = -5.f;
		movement2.z = 0;
	}

	
	modelTransform = glm::translate(modelTransform, movement);
	modelTransform2 = glm::translate(modelTransform2, movement2);

	// Rotate and move our camera based on input
	//myCamera->Rotate(rotation);
	//myCamera->Move(movement);
}

void Vatista::Game::draw(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// glUniform1i(glGetUniformLocation(shader_program, "texSample"), 0);

	myShader->Bind();

	//draw 
	myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * modelTransform);
	myMesh->Draw();
	myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * modelTransform2);
	myMesh->Draw();
	//for (int i = 0; i < meshList.size(); i++) {
	//	meshList[i]->Draw();
	//	//std::cout << 1;
	//}
}
