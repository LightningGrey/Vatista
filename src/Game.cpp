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

	//game loop
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
	//window and camera
	gameWindow = new Vatista::Window(1000, 1000, "Alpha Strike");

	myCamera = std::make_shared<Vatista::Camera>();
	myCamera->SetPosition(glm::vec3(0, 0, 1));
	myCamera->LookAt(glm::vec3(0), glm::vec3(0, 1, 0));
	myCamera->Projection = glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, 0.f, 1000.0f);

	load("./res/init.txt");

	std::string help = "why do i keep losing shit aaaaaaaaa";

	////for morphing
	//std::vector<uint32_t> indices;
	//std::vector<Vertex> vertData;
	//std::vector<uint32_t> indices2;
	//std::vector<Vertex> vertData2;
	//std::vector<MorphVertex> morphVertData;
	//
	////for all static objects
	//std::vector<uint32_t> indices3;
	//std::vector<Vertex> vertData3;
	//
	//bool objectLoad = loader.load("./res/yun_idle_pose_1.obj", indices, vertData);
	//
	////if (objectLoad) {
	////	myMesh = std::make_shared<Mesh>(indices, indices.size(), 
	////		vertData, vertData.size());
	////	meshList.push_back(myMesh);
	////}
	//
	//objectLoad = loader.load("./res/yun_idle_pose_2.obj", indices2, vertData2);
	//
	//if (objectLoad) {
	//	for (int i = 0; i < vertData2.size(); i++) {
	//		morphVertData.push_back(MorphVertex((vertData2[i]), vertData[i].Position,
	//			vertData[i].Normal));
	//	}
	//
	//	myMesh2 = std::make_shared<Mesh>(indices2, indices2.size(),
	//		morphVertData, morphVertData.size());
	//	meshList.push_back(myMesh2);
	//}
	
	////objectLoad = loader.load("./res/GameScene.obj", indices3, vertData3);
	////
	////if (objectLoad) {
	////	myMesh3 = std::make_shared<Mesh>(indices3, indices3.size(),
	////		vertData3, vertData3.size());
	////	meshList.push_back(myMesh3);
	////}

	//player texture
	texture = std::make_shared<Texture>();
	texture->loadFile("./res/yuntexturepaint.png");

	Shader::Sptr phong = std::make_shared<Shader>();
	phong->Load("./res/passthroughMorph.vs", "./res/blinn-phong.fs.glsl"); 

	Shader::Sptr phong2 = std::make_shared<Shader>();
	phong2->Load("./res/lighting.vs.glsl", "./res/blinn-phong.fs.glsl");

	Material::Sptr testMat = std::make_shared<Material>(phong);
	testMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	testMat->Set("a_LightColor", { 0.0f, 1.0f, 0 });
	testMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	testMat->Set("a_AmbientPower", 0.5f);
	testMat->Set("a_LightSpecPower", 0.9f);
	testMat->Set("a_LightShininess", 256.0f);
	testMat->Set("a_LightAttenuation", 0.04f);

	Material::Sptr testMat2 = std::make_shared<Material>(phong2);
	testMat2->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	testMat2->Set("a_LightColor", { 0.0f, 1.0f, 0 });
	testMat2->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	testMat2->Set("a_AmbientPower", 0.5f);
	testMat2->Set("a_LightSpecPower", 0.9f);
	testMat2->Set("a_LightShininess", 256.0f);
	testMat2->Set("a_LightAttenuation", 0.04f);
	//myNormalShader = std::make_shared<Shader>();
	//myNormalShader->Load("./res/passthrough.vs", "./res/blinn-phong.fs.glsl");
	
	//Player 1
	modelTransform = glm::mat4(1.0f);
	modelTransform = glm::rotate(modelTransform, 3.14f, glm::vec3(0, 1, 0));
	pos1 = glm::vec3(-1, 0, 0);
	modelTransform = glm::translate(modelTransform, pos1);
	myScene.emplace_back();
	myScene[0].setPos(pos1);
	myScene[0].setMat(testMat);
	myScene[0].setMesh(myMesh2);
	myScene[0].setRotY(90.0f);
	myScene[0].setCollider(glm::vec2(0.74f, 1.78f));
	p1AtkPos = glm::vec3(0);
	p1AtkCollider = glm::vec2(0.4f);

	//Player 2
	modelTransform2 = glm::mat4(1.0f);
	pos2 = glm::vec3(1, 0, 0);
	modelTransform2 = glm::translate(modelTransform2, pos2);
	myScene.emplace_back();
	myScene[1].setPos(pos2);
	myScene[1].setMat(testMat);
	myScene[1].setMesh(myMesh2);
	myScene[1].setRotY(-90.0f);
	myScene[1].setCollider(glm::vec2(0.74f, 1.78f));
	p2AtkPos = glm::vec3(0);
	p2AtkCollider = glm::vec2(0.4f);

	//pos3 = glm::vec3(1, 0, 0);
	//myScene.emplace_back();
	//myScene[2].Position = pos3;
	//myScene[2].Material = testMat2;
	//myScene[2].Mesh = myMesh3;
	//myScene[2].EulerRotDeg.y = -90.0f;
	//myScene[2].Collider = glm::vec2(0.74f, 1.78f);

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
	bool f = false;
	bool rctrl = false;
	float atkTimer1;
	float atkTimer2;
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
	//movement and speed
	glm::vec3 movement = glm::vec3(0.0f);
	glm::vec3 movement2 = glm::vec3(0.0f);
	float speed = 1.0f;
	
	glfwSetKeyCallback(gameWindow->getWindow(), key_callback);
	glfwSetInputMode(gameWindow->getWindow(), GLFW_STICKY_KEYS, GLFW_TRUE);


	//player 1 movement and dash
	if (glfwGetTime() - kb.tapTimer1 > 0.2f)
		kb.doubleTap1 = false;
	if (glfwGetTime() - kb.tapTimer2 > 0.2f)
		kb.doubleTap2 = false;

	if (kb.a && glfwGetTime() - kb.atkTimer1 > 0.8f) {
		if (!kb.dash1) {
			if (myScene[0].getRot().y == 90.0f)
				movement.x -= speed * 0.001f;
			else if (myScene[0].getRot().y == -90.0f)
				movement.x -= speed * 0.002f;
		}
		if (!dashing1 && myScene[0].getRot().y == 90.0f)
			isBlocking1 = true;
	}
	if (kb.d && glfwGetTime() - kb.atkTimer1 > 0.8f) {
		if (!kb.dash1) {
			if (myScene[0].getRot().y == 90.0f)
				movement.x += speed * 0.002f;
			else if (myScene[0].getRot().y == -90.0f)
				movement.x += speed * 0.001f;
		}
		if (!dashing1 && myScene[0].getRot().y == -90.0f)
			isBlocking1 = true;
	}

	if (kb.left && glfwGetTime() - kb.atkTimer2 > 0.8f) {
		if (!kb.dash2) {
			if (myScene[1].getRot().y == 90.0f)
				movement2.x -= speed * 0.001f;
			else if (myScene[1].getRot().y == -90.0f)
				movement2.x -= speed * 0.002f;
		}
		if (!dashing2 && myScene[1].getRot().y == 90.0f)
			isBlocking2 = true;
	}
	if (kb.right && glfwGetTime() - kb.atkTimer2 > 0.8f) {
		if (!kb.dash2) {
			if (myScene[1].getRot().y == 90.0f)
				movement2.x += speed * 0.002f;
			else if (myScene[1].getRot().y == -90.0f)
				movement2.x += speed * 0.001f;
		}
		if (!dashing2 && myScene[1].getRot().y == -90.0f)
			isBlocking2 = true;
	}

	//attacking
	if (glfwGetTime() - kb.atkTimer1 > 0.8f)
		isAttacking1 = false;
	if (glfwGetTime() - kb.atkTimer2 > 0.8f)
		isAttacking2 = false;
	if (kb.f && !dashing1 && glfwGetTime() - kb.atkTimer1 > 0.8f) {
		isAttacking1 = true;
		kb.atkTimer1 = glfwGetTime();
		movement.x = 0;
	}
	if (kb.rctrl && !dashing2 && glfwGetTime() - kb.atkTimer2 > 0.8f) {
		isAttacking2 = true;
		kb.atkTimer2 = glfwGetTime();
		movement2.x = 0;
	}

	if (isAttacking1 && glfwGetTime() - kb.atkTimer1 > 0.2f && glfwGetTime() - kb.atkTimer1 < 0.6f)
		if (collisionCheck(p1AtkPos, p1AtkCollider, pos2, myScene[1].getCollider()))
			std::cout << "p2 dies" << std::endl;
	if (isAttacking2 && glfwGetTime() - kb.atkTimer2 > 0.2f && glfwGetTime() - kb.atkTimer2 < 0.6f)
		if (collisionCheck(p2AtkPos, p2AtkCollider, pos1, myScene[0].getCollider()))
			std::cout << "p1 dies" << std::endl;

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
		if (collisionCheck(pos1+movement, myScene[0].getCollider(), pos2, myScene[1].getCollider()))
			movement.x = 0;
		pos1 += movement;
	}
	else {
		isBlocking1 = false;
		if (std::floor(lerper1.x*1000)/1000 == std::floor(lerpEnd1.x*1000)/1000) {
			dashing1 = false;
			//std::cout << "done" << std::endl;
			
		}
		else {
			float distCovered = (glfwGetTime() - startTime1) * 0.3f;
			float fractionOfJourney = distCovered / journeyLength1;
			lerper1.x = (1.0 - fractionOfJourney) * lerper1.x + (fractionOfJourney * lerpEnd1.x);
			std::cout << std::floor(lerper1.x * 1000) / 1000 << " " << std::floor(lerpEnd1.x * 1000) / 1000 << std::endl;
		}
		pos1 = lerper1;
		if (collisionCheck(lerpEnd1, myScene[0].getCollider(), pos2, myScene[1].getCollider())){
			if (lerpEnd1.x > pos1.x  && lerpEnd1.x > pos2.x) {
				//std::cout << "rightA" << std::endl;
				lerpEnd1.x = pos2.x + (myScene[1].getCollider().x * 1.02f);
				journeyLength1 = glm::distance(pos1, lerpEnd1);
			}
			else if (lerpEnd1.x < pos1.x && lerpEnd1.x < pos2.x) {
				//std::cout << "leftA" << std::endl;
				lerpEnd1.x = pos2.x - (myScene[1].getCollider().x * 1.02f);
				journeyLength1 = glm::distance(pos1, lerpEnd1);
			}
			if (collisionCheck(pos1, myScene[0].getCollider(), pos2, myScene[1].getCollider())) {
				if (lerpEnd1.x > pos1.x) {
					if (pos1.x < pos2.x) {
						//std::cout << "leftB" << std::endl;
						pos1.x = pos2.x - (myScene[1].getCollider().x);
					}
				}
				else if (lerpEnd1.x < pos1.x) {
					if (pos1.x > pos2.x) {
						//std::cout << "rightB" << std::endl;
						pos1.x = pos2.x + (myScene[1].getCollider().x);
					}
				}
			}
		}
	}

	//edge collisions
	if (pos1.x > 5.63f)
		pos1.x = 5.63f;
	if (pos1.x < -5.63f)
		pos1.x = -5.63f;


	//player 2 movement + dash
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
		if (collisionCheck(pos2 + movement2, myScene[1].getCollider(), pos1, myScene[0].getCollider()))
			movement2.x = 0;
		pos2 += movement2;
	}
	else {
		isBlocking2 = false;
		if (std::floor(lerper2.x * 1000) / 1000 == std::floor(lerpEnd2.x * 1000) / 1000) {
			dashing2 = false;
			//std::cout << "done" << std::endl;
		}
		else {
			float distCovered = (glfwGetTime() - startTime2) * 0.3f;
			float fractionOfJourney = distCovered / journeyLength2;
			lerper2.x = (1.0 - fractionOfJourney) * lerper2.x + (fractionOfJourney * lerpEnd2.x);
			std::cout << lerper2.x << " " << lerpEnd2.x << std::endl;
		}
		pos2 = lerper2;
		if (collisionCheck(lerpEnd2, myScene[1].getCollider(), pos1, myScene[0].getCollider())) {
			if (lerpEnd2.x > pos2.x && lerpEnd2.x > pos1.x) {
				//std::cout << "rightA" << std::endl;
				lerpEnd2.x = pos1.x + (myScene[0].getCollider().x * 1.02f);
				journeyLength2 = glm::distance(pos2, lerpEnd2);
			}
			else if (lerpEnd2.x < pos2.x && lerpEnd2.x < pos1.x) {
				//std::cout << "leftA" << std::endl;
				lerpEnd2.x = pos1.x - (myScene[0].getCollider().x * 1.02f);
				journeyLength2 = glm::distance(pos2, lerpEnd2);
			}
			if (collisionCheck(pos2, myScene[1].getCollider(), pos1, myScene[0].getCollider())) {
				if (lerpEnd2.x > pos2.x) {
					if (pos2.x < pos1.x) {
						//std::cout << "leftB" << std::endl;
						pos2.x = pos1.x - (myScene[0].getCollider().x);
					}
				}
				else if (lerpEnd2.x < pos2.x) {
					if (pos2.x > pos1.x) {
						//std::cout << "rightB" << std::endl;
						pos2.x = pos1.x + (myScene[0].getCollider().x);
					}
				}
			}
		}
	}
	if (pos2.x > 5.63f)
		pos2.x = 5.63f;
	if (pos2.x < -5.63f)
		pos2.x = -5.63f;

	//switching sides after dash
	if (pos1.x > pos2.x) {
		myScene[0].setRotY(-90.0f);
		myScene[1].setRotY(90.0f);
		p1AtkPos.x = pos1.x - myScene[0].getCollider().x - p1AtkCollider.x;
		p2AtkPos.x = pos2.x + myScene[1].getCollider().x + p2AtkCollider.x;
	}
	else {
		myScene[0].setRotY(90.0f);
		myScene[1].setRotY(-90.0f);
		p1AtkPos.x = pos1.x + myScene[0].getCollider().x + p1AtkCollider.x;
		p2AtkPos.x = pos2.x - myScene[1].getCollider().x - p2AtkCollider.x;
	}
	myScene[0].setPos(pos1);
	myScene[1].setPos(pos2);

	//Player1->getCharacter()->update(dt, keyPress);
	//Player2->getCharacter()->update(dt, keyPress);
}

void Vatista::Game::draw(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw 
	for (int i = 0; i < myScene.size(); i++) {
		myScene[i].Draw(myCamera);
	}
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

bool Vatista::Game::load(std::string filename)
{
	std::string line;

	//vector of all data to load
	std::vector<std::string> dataList;

	loadingFile.open(filename, std::ios::in | std::ios::binary);
	if (!loadingFile) {
		VATISTA_LOG_ERROR("No file");
		throw new std::runtime_error("File open failed");
		return false;
	}

	while (std::getline(loadingFile, line)) {
		dataList.push_back(line);
	}
	loadingFile.close();

	for (int i = 0; i < dataList.size(); i++) {
		dataFile.open("./res/" + dataList[i], std::ios::in | std::ios::binary);
		if (!dataFile) {
			VATISTA_LOG_ERROR("No file");
			throw new std::runtime_error("File open failed");
			return false;
		}

		int animBuffer; //check for static obj vs moving
		int vertTotalBuffer; //check for verts total
		int indiceTotalBuffer; //check for indices total
		LoadMorphVertex* morphBuffer;
		uint32_t* indicesBuffer;

		std::cout << "reading now" << std::endl;

		dataFile.read((char*)&animBuffer, sizeof(int));
		dataFile.read((char*)&vertTotalBuffer, sizeof(int));
		dataFile.read((char*)&indiceTotalBuffer, sizeof(int));

		morphBuffer = new LoadMorphVertex[vertTotalBuffer];
		indicesBuffer = new uint32_t[indiceTotalBuffer];

		//access using pointer (*(value+n))
		dataFile.read((char*)morphBuffer, sizeof(LoadMorphVertex) * vertTotalBuffer);
		dataFile.read((char*)indicesBuffer, sizeof(uint32_t) * indiceTotalBuffer);

		dataFile.close();

		std::vector<uint32_t> indices;
		std::vector<Vertex> vertData;
		std::vector<MorphVertex> morphVertData;

		for (int i = 0; i < vertTotalBuffer; i++) {
			MorphVertex morph = { Vertex{(*(morphBuffer + i)).Position,
			(*(morphBuffer + i)).UV, (*(morphBuffer + i)).Normal},
				(*(morphBuffer + i)).PositionS, (*(morphBuffer + i)).NormalS };
			morphVertData.push_back(morph);
		}


		for (int i = 0; i < indiceTotalBuffer; i++) {
			indices.push_back(*(indicesBuffer + i));
		}

		myMesh2 = std::make_shared<Mesh>(indices, indices.size(), 
			morphVertData, morphVertData.size());
		meshList.push_back(myMesh2);
	}

	return true;
}

//bool Vatista::Game::doubleTap = false;
void Vatista::Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_A:
			if (kb.doubleTap1 && kb.tap1 == key && glfwGetTime() - kb.atkTimer1 > 0.8f) {
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
			if (kb.doubleTap1 && kb.tap1 == key && glfwGetTime() - kb.atkTimer1 > 0.8f) {
				//std::cout << "dash" << std::endl;
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
			if (kb.doubleTap2 && kb.tap2 == key && glfwGetTime() - kb.atkTimer2 > 0.8f) {
				//std::cout << "dash" << std::endl;
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
			if (kb.doubleTap2 && kb.tap2 == key && glfwGetTime() - kb.atkTimer2 > 0.8f) {
				//std::cout << "dash" << std::endl;
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
		case GLFW_KEY_F:
			std::cout << "attack1" << std::endl;
			kb.f = true; break;
		case GLFW_KEY_RIGHT_CONTROL:
			std::cout << "attack2" << std::endl;
			kb.rctrl = true; break;
		default: break;
		}
	}
	if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_A: kb.a = false; if (kb.dash1) { kb.dash1 = false; } break;
		case GLFW_KEY_D: kb.d = false; if (kb.dash1) { kb.dash1 = false; } break;
		case GLFW_KEY_LEFT: kb.left = false; if (kb.dash2) { kb.dash2 = false; } break;
		case GLFW_KEY_RIGHT: kb.right = false; if (kb.dash2) { kb.dash2 = false; } break;
		case GLFW_KEY_F: kb.f = false; break;
		case GLFW_KEY_RIGHT_CONTROL: kb.rctrl = false; break;
		default: break;
		}
	}

}
