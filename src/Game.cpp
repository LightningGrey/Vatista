#include "Game.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include <iostream>

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

	bool objectLoad = loader.load("./res/test_model.obj", vertices, UVs, normals, indices, vertData);

	if (objectLoad) {
		myMesh = std::make_shared<Mesh>(vertices, vertices.size(), UVs,
			UVs.size(), normals, normals.size(), indices, indices.size(),
			vertData, vertData.size());
		meshList.push_back(myMesh);
	}

	//myShader = std::make_shared<Shader>();
	//myShader->Load("./res/passthrough.vs", "./res/passthrough.fs");

	texture = std::make_shared<Texture>();
	texture->loadFile("./res/color-grid.png");

	Shader_sptr phong = std::make_shared<Shader>();
	phong->Load("./res/lighting.vs.glsl", "./res/blinn-phong.fs.glsl");

	//GAME_LOG_INFO(glGetString(GL_RENDERER));
	//GAME_LOG_INFO(glGetString(GL_VERSION));
	//
	//for (int i = 0; i < vertData.size(); i++) {
	//	std::cout << vertData.at(i).Position.x;
	//	std::cout << vertData.at(i).Position.y;
	//	std::cout << vertData.at(i).Position.z << std::endl;
	//
	//}

	Material::Sptr testMat = std::make_shared<Material>(phong);
	testMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	testMat->Set("a_LightColor", { 0.0f, 1.0f, 0 });
	testMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	testMat->Set("a_AmbientPower", 0.5f);
	testMat->Set("a_LightSpecPower", 0.9f);
	testMat->Set("a_LightShininess", 256.0f);
	testMat->Set("a_LightAttenuation", 0.04f);
	//myNormalShader = std::make_shared<Shader>();
	//myNormalShader->Load("./res/passthrough.vs", "./res/normalView.fs");

	modelTransform = glm::mat4(1.0f);
	modelTransform = glm::rotate(modelTransform, 3.14f, glm::vec3(0, 1, 0));
	pos1 = glm::vec3(0, 0, 1.0f);
	modelTransform = glm::translate(modelTransform, pos1);
	myScene.emplace_back();
	myScene[0].Position = pos1;
	myScene[0].Material = testMat;
	myScene[0].Mesh = myMesh;
	myScene[0].EulerRotDeg.y = 180.0f;

	modelTransform2 = glm::mat4(1.0f);
	pos2 = glm::vec3(0, 0, -1.0f);
	modelTransform2 = glm::translate(modelTransform2, pos2);
	myScene.emplace_back();
	myScene[1].Position = pos2;
	myScene[1].Material = testMat;
	myScene[1].Mesh = myMesh;
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
		movement.z += speed * 0.001f;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		movement.z -= speed * 0.002f;

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
		movement2.z += speed * 0.002f;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		movement2.z -= speed * 0.001f;

	if (!dashing1) {
		if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
			lerpEnd = pos1 + glm::vec3(0, 0, 0.02f);
			dashing1 = true;
			startTime = dt;
			journeyLength = glm::distance(pos1, lerpEnd);
		}
		if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_E) == GLFW_REPEAT) {
			lerpEnd = pos1 - glm::vec3(0, 0, 0.02f);
			dashing1 = true;
			startTime = dt;
			journeyLength = glm::distance(pos1, lerpEnd);
		}
		pos1 += movement;
	}
	else {
		float distCovered = (dt - startTime) * 1.0f;
		float fractionOfJourney = distCovered / journeyLength;
		pos1.z = (1.0 - fractionOfJourney) * lerpEnd.z + (fractionOfJourney * pos1.z);
		if (pos1.z == lerpEnd.z){
			dashing1 = false;
			startTime = dt;
			journeyLength = glm::distance(pos1, lerpEnd);
		}
	}
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



	myScene[0].Position = pos1;
	myScene[1].Position = pos2;

	// Rotate and move our camera based on input
	//myCamera->Rotate(rotation);
	//myCamera->Move(movement);
}

void Vatista::Game::draw(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//myShader->Bind();

	//draw 
	for (int i = 0; i < myScene.size(); i++) {
		myScene[i].Draw(myCamera);
	}
	//myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * modelTransform2);
	//myMesh->Draw();
	//for (int i = 0; i < meshList.size(); i++) {
	//	meshList[i]->Draw();
	//	//std::cout << 1;
	//}
}