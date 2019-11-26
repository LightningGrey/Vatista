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

	std::vector<uint32_t> indices;
	std::vector<Vertex> vertData;

	bool objectLoad = loader.load("./res/test_model.obj", indices, vertData);

	if (objectLoad) {
		myMesh = std::make_shared<Mesh>(indices, indices.size(), 
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

	//modelTransform3 = glm::mat4(1.0f);
	//modelTransform3 = glm::rotate(modelTransform3, 3.14f, glm::vec3(0, 1, 0));

	//position for centre of weapon hitbox and character hurtbox
	hitboxPos1 = glm::vec3(pos1.x, pos1.y, pos1.z + 0.2f);
	collisionboxPos1 = glm::vec3(pos1.x, pos1.y, pos1.z);
	//position for centre of weapon hitbox and character hurtbox
	hitboxPos2 = glm::vec3();
	collisionboxPos2 = glm::vec3();

	glEnable(GL_CULL_FACE);
}

void Vatista::Game::close()
{
	glfwTerminate();
}


void Vatista::Game::update(float dt)
{
	glm::vec3 movement = glm::vec3(0.0f);
	glm::vec3 movement2 = glm::vec3(0.0f);

	float speed = 1.0f;
	float rotSpeed = 1.0f;

	bool p1atk;
	bool p2atk;

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		movement.z += speed * 0.001f;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		movement.z -= speed * 0.002f;

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
		movement2.z += speed * 0.002f;
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		movement2.z -= speed * 0.001f;

	//Attack buttons (Space bar for player 1 and right ctrl for player 2)
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		p1atk = true;
	else
		p1atk = false;

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		p2atk = true;
	else
		p2atk = false;

	//Creating hitboxes

	//This is an idea, not what's this code does
	/*Create hitbox at the characters' weapons if they are attacking
	 if player's state is attacking,
	 get the position of a vector in the mesh of the weapon attack
	 then transform the hitbox to fit over where that vector is
	*/
	//if (player1.state == attacking) {}

	//else {}        //if not attacking, then hitbox can be moved/unloaded

	//the position of the hitboxes are slightly in front of the player
	hitboxPos1 = pos1 + 0.6f;
	collisionboxPos1 = pos1;
	hitboxPos2 = pos2 + 0.6f;
	collisionboxPos2 = pos2;

	float length = 0.2f; //square hitbox length from centre to sides


	//Collisions

	//in here, do the AABB collisions on
	//player1's hitbox + player2's collisionbox
	//player2's hitbox + player1's collisionbox
	//player1's collisionbox + player2's collisionbox (to prevent them phasing through each other)
	//might be better to make a single function to run all collision checks needed

	if (collisionCheck(hitboxPos1, collisionboxPos2, length) && p1atk)
	{
		//player 2 is hit
		std::cout << "player 2 is hit" << std::endl;
	}
	if (collisionCheck(collisionboxPos1, hitboxPos2, length) && p2atk)
	{
		//player 1 is hit
		std::cout << "player 1 is hit" << std::endl;
	}

	if (collisionCheck(collisionboxPos1, collisionboxPos2, 0.5f)) //substitue 0.5f with the value of the player's box
	{
		//players are touching each other
		//std::cout << "colliding" << std::endl;

		if (movement.z >= 0 && movement2.z >= 0)
		{
			movement.z = 0;
			movement2.z = 0;
		}
		else if (movement2.z < 0 && movement.z >= 0) //player 1 approaching 2
		{
			movement.z = 0;
		}
		else if (movement.z < 0 && movement2.z >= 0) //player 2 approaching 1
		{
			movement2.z = 0;
		}

		if (p1atk)
		{
			//player 2 is hit
			std::cout << "player 2 is hit" << std::endl;
		}
		if (p2atk)
		{
			//player 1 is hit
			std::cout << "player 1 is hit" << std::endl;
		}
	}
	if (!dashing1) {
		if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
			lerpEnd = pos1 + glm::vec3(0, 0, 0.02f);
			dashing1 = true;
			startTime = dt;
			journeyLength = glm::distance(pos1, lerpEnd);
		}
		if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_E) == GLFW_PRESS) {
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
	
	modelTransform = glm::translate(modelTransform, movement);
	modelTransform2 = glm::translate(modelTransform2, movement2);


	myScene[0].Position = pos1;
	myScene[1].Position = pos2;
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
	//for (int i = 0; i < meshList.size(); i++) {
	//	meshList[i]->Draw();
	//	//std::cout << 1;
	//}
}

bool Vatista::Game::collisionCheck(glm::vec3 obj1, glm::vec3 obj2, float len)
{
	//std::cout << "           obj1.z = " << obj1.z << std::endl;
	//std::cout << "                         obj2.z = " << obj2.z << std::endl;

	if (obj1.z < 0 && obj2.z > 0)
	{
		obj1.z = abs(obj1.z);
	}
	else if (obj1.z > 0 && obj2.z < 0)
	{
		obj2.z = abs(obj2.z);
	}
	else if (obj1.z < 0 && obj2.z < 0)
	{
		if (abs(obj1.z + obj2.z) <= len)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool collisionX = obj1.z + len >= obj2.z &&
		obj2.z + len >= obj1.z;

	return collisionX;
}