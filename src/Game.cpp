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
	myCamera->SetPosition(glm::vec3(0, 0, 1));
	myCamera->LookAt(glm::vec3(0), glm::vec3(0, 1, 0));
	myCamera->Projection = glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, 0.f, 1000.0f);

	std::vector<uint32_t> indices;
	std::vector<Vertex> vertData;
	std::vector<uint32_t> indices2;
	std::vector<Vertex> vertData2;
	std::vector<uint32_t> morphIndices;
	std::vector<MorphVertex> morphVertData;

	bool objectLoad = loader.load("./res/test_model.obj", indices, vertData);

	if (objectLoad) {
		myMesh = std::make_shared<Mesh>(indices, indices.size(), 
			vertData, vertData.size());
		meshList.push_back(myMesh);
	}

	//objectLoad = loader.load("./res/test_model.obj", indices2, vertData2);
	//
	//if (objectLoad) {
	//	for (int i = 0; i < vertData2.size(); i++) {
	//		morphVertData.push_back(MorphVertex((vertData2[i]), vertData[i].Position,
	//			vertData[i].Normal));
	//	}
	//
	//	myMesh2 = std::make_shared<Mesh>(indices2, indices2.size(),
	//		vertData2, vertData2.size());
	//	meshList.push_back(myMesh2);
	//}


	//player texture
	texture = std::make_shared<Texture>();
	texture->loadFile("./res/color-grid.png");

	Shader::Sptr phong = std::make_shared<Shader>();
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
	pos1 = glm::vec3(-1, 0, 0);
	modelTransform = glm::translate(modelTransform, pos1);
	myScene.emplace_back();
	myScene[0].Position = pos1;
	myScene[0].Material = testMat;
	myScene[0].Mesh = myMesh;
	myScene[0].EulerRotDeg.y = 90.0f;
	myScene[0].Collider = glm::vec2(0.74f, 1.78f);

	//right player
	modelTransform2 = glm::mat4(1.0f);
	pos2 = glm::vec3(1, 0, 0);
	modelTransform2 = glm::translate(modelTransform2, pos2);
	myScene.emplace_back();
	myScene[1].Position = pos2;
	myScene[1].Material = testMat;
	myScene[1].Mesh = myMesh;
	myScene[1].EulerRotDeg.y = -90.0f;
	myScene[1].Collider = glm::vec2(0.74f, 1.78f);

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

struct keyboard {
	bool a = false;
	bool d = false;
	bool left = false;
	bool right = false;
	bool space = false;
	int tap1;
	int tap2;
	bool doubleTap1 = false;
	bool doubleTap2 = false;
	bool dash1 = false;
	bool dash2 = false;
	float tapTimer1;
	float tapTimer2;
}kb;
void Vatista::Game::update(float dt)
{
	glm::vec3 movement = glm::vec3(0.0f);
	glm::vec3 movement2 = glm::vec3(0.0f);

	float speed = 1.0f;
	float rotSpeed = 1.0f;

	bool p1atk;
	bool p2atk;

	if (glfwGetTime() - kb.tapTimer1 > 0.2f)
		kb.doubleTap1 = false;
	if (glfwGetTime() - kb.tapTimer2 > 0.2f)
		kb.doubleTap2 = false;



	if (kb.a) {
		if (!kb.dash1) {
			if (myScene[0].EulerRotDeg.y == 90.0f)
				movement.x -= speed * 0.001f;
			else if (myScene[0].EulerRotDeg.y == -90.0f)
				movement.x -= speed * 0.002f;
		}
		if (!dashing1 && myScene[0].EulerRotDeg.y == 90.0f)
			isBlocking1 = true;
	}
	if (kb.d) {
		if (!kb.dash1) {
			if (myScene[0].EulerRotDeg.y == 90.0f)
				movement.x += speed * 0.002f;
			else if (myScene[0].EulerRotDeg.y == -90.0f)
				movement.x += speed * 0.001f;
		}
		if (!dashing1 && myScene[0].EulerRotDeg.y == -90.0f)
			isBlocking1 = true;
	}

	if (kb.left) {
		if (!kb.dash2) {
			if (myScene[1].EulerRotDeg.y == 90.0f)
				movement2.x -= speed * 0.001f;
			else if (myScene[1].EulerRotDeg.y == -90.0f)
				movement2.x -= speed * 0.002f;
		}
		if (!dashing2 && myScene[1].EulerRotDeg.y == 90.0f)
			isBlocking2 = true;
	}
	if (kb.right) {
		if (!kb.dash2) {
			if (myScene[1].EulerRotDeg.y == 90.0f)
				movement2.x += speed * 0.002f;
			else if (myScene[1].EulerRotDeg.y == -90.0f)
				movement2.x += speed * 0.001f;
		}
		if (!dashing2 && myScene[1].EulerRotDeg.y == -90.0f)
			isBlocking2 = true;
	}

	glfwSetKeyCallback(gameWindow->getWindow(), key_callback);
	glfwSetInputMode(gameWindow->getWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);

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
	hitboxPos2 = pos2 + 0.6f;

	float length = 0.2f; //square hitbox length from centre to sides

	//
	//	if (p1atk)
	//	{
	//		//player 2 is hit
	//		std::cout << "player 2 is hit" << std::endl;
	//	}
	//	if (p2atk)
	//	{
	//		//player 1 is hit
	//		std::cout << "player 1 is hit" << std::endl;
	//	}
	//}

	if (!dashing1) {
		if (kb.dash1 && kb.tap1 == GLFW_KEY_A) {
			lerpEnd1 = pos1 - glm::vec3(2.f, 0, 0);
			dashing1 = true;
		}
		if (kb.dash1 && kb.tap1 == GLFW_KEY_D) {
			lerpEnd1 = pos1 + glm::vec3(2.f, 0, 0);
			dashing1 = true;
		}
		lerper1 = pos1;
		startTime1 = glfwGetTime();
		journeyLength1 = glm::distance(pos1, lerpEnd1);
		kb.dash1 = false;
		if (collisionCheck(pos1+movement, myScene[0].Collider, pos2, myScene[1].Collider))
			movement.x = 0;
		pos1 += movement;
	}
	else {
		isBlocking1 = false;
		if (std::floor(lerper1.x*1000)/1000 == std::floor(lerpEnd1.x*1000)/1000) {
			dashing1 = false;
			std::cout << "done" << std::endl;
			
		}
		else {
			float distCovered = (glfwGetTime() - startTime1) * 0.3f;
			float fractionOfJourney = distCovered / journeyLength1;
			lerper1.x = (1.0 - fractionOfJourney) * lerper1.x + (fractionOfJourney * lerpEnd1.x);
			std::cout << std::floor(lerper1.x * 1000) / 1000 << " " << std::floor(lerpEnd1.x * 1000) / 1000 << std::endl;
		}
		pos1 = lerper1;
		if (collisionCheck(lerpEnd1, myScene[0].Collider, pos2, myScene[1].Collider)){
			if (lerpEnd1.x > pos1.x  && lerpEnd1.x > pos2.x) {
				std::cout << "rightA" << std::endl;
				lerpEnd1.x = pos2.x + (myScene[1].Collider.x * 1.02f);
				journeyLength1 = glm::distance(pos1, lerpEnd1);
			}
			else if (lerpEnd1.x < pos1.x && lerpEnd1.x < pos2.x) {
				std::cout << "leftA" << std::endl;
				lerpEnd1.x = pos2.x - (myScene[1].Collider.x * 1.02f);
				journeyLength1 = glm::distance(pos1, lerpEnd1);
			}
			if (collisionCheck(pos1, myScene[0].Collider, pos2, myScene[1].Collider)) {
				if (lerpEnd1.x > pos1.x) {
					if (pos1.x < pos2.x) {
						std::cout << "leftB" << std::endl;
						pos1.x = pos2.x - (myScene[1].Collider.x);
					}
				}
				else if (lerpEnd1.x < pos1.x) {
					if (pos1.x > pos2.x) {
						std::cout << "rightB" << std::endl;
						pos1.x = pos2.x + (myScene[1].Collider.x);
					}
				}
			}
		}
	}

	if (pos1.x > 5.63f)
		pos1.x = 5.63f;
	if (pos1.x < -5.63f)
		pos1.x = -5.63f;

	if (!dashing2) {
		if (kb.dash2 && kb.tap2 == GLFW_KEY_LEFT) {
			lerpEnd2 = pos2 - glm::vec3(2.f, 0, 0);
			dashing2 = true;
		}
		if (kb.dash2 && kb.tap2 == GLFW_KEY_RIGHT) {
			lerpEnd2 = pos2 + glm::vec3(2.f, 0, 0);
			dashing2 = true;
		}
		lerper2 = pos2;
		startTime2 = glfwGetTime();
		journeyLength2 = glm::distance(pos2, lerpEnd2);
		kb.dash2 = false;
		if (collisionCheck(pos2 + movement2, myScene[1].Collider, pos1, myScene[0].Collider))
			movement2.x = 0;
		pos2 += movement2;
	}
	else {
		isBlocking2 = false;
		if (std::floor(lerper2.x * 1000) / 1000 == std::floor(lerpEnd2.x * 1000) / 1000) {
			dashing2 = false;
			std::cout << "done" << std::endl;
		}
		else {
			float distCovered = (glfwGetTime() - startTime2) * 0.3f;
			float fractionOfJourney = distCovered / journeyLength2;
			lerper2.x = (1.0 - fractionOfJourney) * lerper2.x + (fractionOfJourney * lerpEnd2.x);
			std::cout << lerper2.x << " " << lerpEnd2.x << std::endl;
		}
		pos2 = lerper2;
		if (collisionCheck(lerpEnd2, myScene[1].Collider, pos1, myScene[0].Collider)) {
			if (lerpEnd2.x > pos2.x && lerpEnd2.x > pos1.x) {
				std::cout << "rightA" << std::endl;
				lerpEnd2.x = pos1.x + (myScene[0].Collider.x * 1.02f);
				journeyLength2 = glm::distance(pos2, lerpEnd2);
			}
			else if (lerpEnd2.x < pos2.x && lerpEnd2.x < pos1.x) {
				std::cout << "leftA" << std::endl;
				lerpEnd2.x = pos1.x - (myScene[0].Collider.x * 1.02f);
				journeyLength2 = glm::distance(pos2, lerpEnd2);
			}
			if (collisionCheck(pos2, myScene[1].Collider, pos1, myScene[0].Collider)) {
				if (lerpEnd2.x > pos2.x) {
					if (pos2.x < pos1.x) {
						std::cout << "leftB" << std::endl;
						pos2.x = pos1.x - (myScene[0].Collider.x);
					}
				}
				else if (lerpEnd2.x < pos2.x) {
					if (pos2.x > pos1.x) {
						std::cout << "rightB" << std::endl;
						pos2.x = pos1.x + (myScene[0].Collider.x);
					}
				}
			}
		}
	}
	if (pos2.x > 5.63f)
		pos2.x = 5.63f;
	if (pos2.x < -5.63f)
		pos2.x = -5.63f;

	if (pos1.x > pos2.x) {
		myScene[0].EulerRotDeg.y = -90.0f;
		myScene[1].EulerRotDeg.y = 90.0f;
	}
	else {
		myScene[0].EulerRotDeg.y = 90.0f;
		myScene[1].EulerRotDeg.y = -90.0f;
	}
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

bool Vatista::Game::collisionCheck(glm::vec3 x, glm::vec2 collider1, glm::vec3 y, glm::vec2 collider2)
{
	//std::cout << "           obj1.z = " << obj1.z << std::endl;
	//std::cout << "                         obj2.z = " << obj2.z << std::endl;
	if (x.x < y.x + collider2.x && x.x + collider1.x > y.x&& x.y < y.y + collider2.y && x.y + collider1.y > y.y) {
		return true;
	}
	else
		return false;
	
}

//bool Vatista::Game::doubleTap = false;
void Vatista::Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
	//	if (doubleTap) {
	//		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
	//			std::cout << "space" << std::endl;
	//			doubleTap = false;
	//		}
	//	}
	//	else {
	//		doubleTap = true;
	//		kb.tapTimer = glfwGetTime();
	//	}
	//}

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_A:
			if (kb.doubleTap1 && kb.tap1 == key) {
				std::cout << "dash" << std::endl;
				kb.dash1 = true;
				kb.doubleTap1 = false;
			}
			else {
				kb.tapTimer1 = glfwGetTime();
				kb.doubleTap1 = true;
				kb.tap1 = key;
			}
			kb.a = true;
			break;
		case GLFW_KEY_D: 
			if (kb.doubleTap1 && kb.tap1 == key) {
				std::cout << "dash" << std::endl;
				kb.dash1 = true;
				kb.doubleTap1 = false;
			}
			else {
				kb.tapTimer1 = glfwGetTime();
				kb.doubleTap1 = true;
				kb.tap1 = key;
			}
			kb.d = true;
			break;
		case GLFW_KEY_LEFT:
			if (kb.doubleTap2 && kb.tap2 == key) {
				std::cout << "dash" << std::endl;
				kb.dash2 = true;
				kb.doubleTap2 = false;
			}
			else {
				kb.tapTimer2 = glfwGetTime();
				kb.doubleTap2 = true;
				kb.tap2 = key;
			}
			kb.left = true;
			break;
		case GLFW_KEY_RIGHT:
			if (kb.doubleTap2 && kb.tap2 == key) {
				std::cout << "dash" << std::endl;
				kb.dash2 = true;
				kb.doubleTap2 = false;
			}
			else {
				kb.tapTimer2 = glfwGetTime();
				kb.doubleTap2 = true;
				kb.tap2 = key;
			}
			kb.right = true;
			break;
		case GLFW_KEY_SPACE: kb.space = true; break;
		default: break;
		}
	}
	if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_A: kb.a = false; if (kb.dash1) { kb.dash1 = false; } break;
		case GLFW_KEY_D: kb.d = false; if (kb.dash1) { kb.dash1 = false; } break;
		case GLFW_KEY_LEFT: kb.left = false; if (kb.dash2) { kb.dash2 = false; } break;
		case GLFW_KEY_RIGHT: kb.right = false; if (kb.dash2) { kb.dash2 = false; } break;
		case GLFW_KEY_SPACE: kb.space = false; break;
		default: break;
		}
	}

}
