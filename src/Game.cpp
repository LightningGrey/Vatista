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
	C1 = Character(true, myMesh2, testMat2);

	//Player 2
	C2 = Character(false, myMesh2, testMat2);

	//pos3 = glm::vec3(1, 0, 0);
	//myScene.emplace_back();
	TestMap.setPos(glm::vec3(0));
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

void Vatista::Game::update(float dt)
{
	C1.update(dt, gameWindow->getWindow(), C2);
	C2.update(dt, gameWindow->getWindow(), C1);
}

void Vatista::Game::draw(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw 
	//TestMap.Draw(myCamera);
	C1.Draw(myCamera);
	C2.Draw(myCamera);
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
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.pop_back();
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

