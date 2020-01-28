#include "Game.h" 
#include "SceneManager.h" 
#include "MeshRenderer.h" 
#include <iostream> 
#include "Math.h"

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
	gameWindow = new Vatista::Window(1920, 1080, "Alpha Strike");

	myCamera = std::make_shared<Vatista::Camera>();
	myCamera->SetPosition(glm::vec3(0, 1, 10));
	myCamera->LookAt(glm::vec3(0), glm::vec3(0, 30, 0));
	myCamera->Projection = glm::perspective(45.0f, 16.f / 9.f, 1.0f, 150.0f);

	audioEngine.Init();
	audioEngine.LoadBank("./res/Master", FMOD_STUDIO_LOAD_BANK_NORMAL);
	audioEngine.LoadEvent("Music", "{7cd3c8cb-a575-4dd4-bf24-92345bcb126d}");
	audioEngine.PlayEvent("Music");
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

	////player texture 
	//texture = std::make_shared<Texture>(); 
	////texture->loadFile("./res/yuntexturepaint.png"); 
	//texture->loadFile("./res/base.png"); 
	//texture->bind(1); 
	// 
	////normal map test 
	//texture2 = std::make_shared<Texture>(); 
	//texture2->loadFile("./res/default.png"); 
	//texture2->bind(2); 

	texture = std::make_shared<Texture>();
	texture->loadFile("./res/yuntexturepaint.png");

	texture2 = std::make_shared<Texture>();
	texture2->loadFile("./res/FIXED_TRAINING_MAP/Training_Map_Texture.png");

	texture3 = std::make_shared<Texture>();
	texture3->loadFile("./res/default.png");

	Shader::Sptr phong = std::make_shared<Shader>();
	phong->Load("./res/passthroughMorph.vs", "./res/blinn-phong.fs.glsl");

	Shader::Sptr phong2 = std::make_shared<Shader>();
	phong2->Load("./res/lighting.vs.glsl", "./res/blinn-phong.fs.glsl");

	Material::Sptr testMat = std::make_shared<Material>(phong);
	testMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	testMat->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
	testMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	testMat->Set("a_AmbientPower", 0.5f);
	testMat->Set("a_LightSpecPower", 0.9f);
	testMat->Set("a_LightShininess", 256.0f);
	testMat->Set("a_LightAttenuation", 0.04f);
	testMat->Set("texSample", texture);

	Material::Sptr testMat2 = std::make_shared<Material>(phong2);
	testMat2->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	testMat2->Set("a_LightColor", { 0.0f, 1.0f, 0 });
	testMat2->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	testMat2->Set("a_AmbientPower", 0.5f);
	testMat2->Set("a_LightSpecPower", 0.9f);
	testMat2->Set("a_LightShininess", 256.0f);
	testMat2->Set("a_LightAttenuation", 0.04f);
	testMat2->Set("texSample", texture2);


	////Player 1 
	//modelTransform = glm::mat4(1.0f); 
	//modelTransform = glm::rotate(modelTransform, 3.14f, glm::vec3(0, 1, 0)); 
	//pos1 = glm::vec3(-1, 0, 0); 
	//modelTransform = glm::translate(modelTransform, pos1); 
	//myScene.emplace_back(); 
	//myScene[0].setPos(pos1); 
	//myScene[0].setMat(testMat2); 
	//myScene[0].setMesh(myMesh2); 
	//myScene[0].setRotY(90.0f); 
	//myScene[0].setCollider(glm::vec2(0.74f, 1.78f)); 
	//p1AtkPos = glm::vec3(0); 
	//p1AtkCollider = glm::vec2(0.4f); 

	//pos3 = glm::vec3(1, 0, 0); 
	//myScene.emplace_back(); 
	TestMap = std::make_shared<GameObject>();
	TestMap->setPos(glm::vec3(0, -1.f, 1));
	TestMap->setMesh(meshList[1]);
	TestMap->setMat(testMat2);
	TestMap->setRotY(-90.f);
	TestMap->setTexture(texture2);
	TestMap->setScale(glm::vec3(0.3f));
	ObjectList.push_back(TestMap);
	//myScene[2].Position = pos3; 
	//myScene[2].Material = testMat2; 
	//myScene[2].Mesh = myMesh3; 
	//myScene[2].EulerRotDeg.y = -90.0f; 
	//myScene[2].Collider = glm::vec2(0.74f, 1.78f); 

	C1 = std::make_shared<Character>(true, meshList[0], testMat);
	ObjectList.push_back(C1);

	//Player 2 
	C2 = std::make_shared<Character>(false, meshList[0], testMat);
	ObjectList.push_back(C2);


	glEnable(GL_CULL_FACE);
}

void Vatista::Game::close()
{
	glfwTerminate();
	audioEngine.Shutdown();
}

void Vatista::Game::update(float dt)
{
	C1->update(dt, gameWindow->getWindow(), C2);
	C2->update(dt, gameWindow->getWindow(), C1);
	audioEngine.Update();
}

void Vatista::Game::draw(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw  
	for (auto object : ObjectList) {
		object->Draw(myCamera);
	}

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

		//int animBuffer; //check for static obj vs moving 
		//int vertTotalBuffer; //check for verts total 
		//int indiceTotalBuffer; //check for indices total 
		//Vertex* vertBuffer = nullptr; 
		//LoadMorphVertex* morphBuffer = nullptr; 
		//uint32_t* indicesBuffer; 
		// 
		//std::cout << "reading now" << std::endl; 
		// 
		//dataFile.read((char*)&animBuffer, sizeof(int)); 
		//dataFile.read((char*)&vertTotalBuffer, sizeof(int)); 
		//dataFile.read((char*)&indiceTotalBuffer, sizeof(int)); 
		// 
		////access buffers using pointer (*(value+n)) 
		//if (animBuffer) { 
		//	morphBuffer = new LoadMorphVertex[vertTotalBuffer]; 
		//	dataFile.read((char*)morphBuffer, sizeof(LoadMorphVertex) * vertTotalBuffer); 
		//} else { 
		//	vertBuffer = new Vertex[vertTotalBuffer]; 
		//	dataFile.read((char*)vertBuffer, sizeof(Vertex) * vertTotalBuffer); 
		//} 
		//indicesBuffer = new uint32_t[indiceTotalBuffer]; 
		//dataFile.read((char*)indicesBuffer, sizeof(uint32_t) * indiceTotalBuffer); 
		// 
		//dataFile.close(); 
		// 
		//std::vector<uint32_t> indices; 
		//std::vector<Vertex> vertData; 
		//std::vector<MorphVertex> morphVertData; 
		// 
		//for (int i = 0; i < indiceTotalBuffer; i++) { 
		//	indices.push_back(*(indicesBuffer + i)); 
		//} 
		// 
		//if (animBuffer) { 
		//	for (int i = 0; i < vertTotalBuffer; i++) { 
		//		MorphVertex morph = { Vertex{(*(morphBuffer + i)).Position, 
		//		(*(morphBuffer + i)).UV, (*(morphBuffer + i)).Normal}, 
		//			(*(morphBuffer + i)).PositionS, (*(morphBuffer + i)).NormalS }; 
		//		morphVertData.push_back(morph); 
		// 
		//		myMesh = std::make_shared<Mesh>(indices, indices.size(), 
		//			morphVertData, morphVertData.size()); 
		//		meshList.push_back(myMesh); 
		//	} 
		//}else{ 
		//	for (int i = 0; i < vertTotalBuffer; i++) { 
		//		Vertex nomorph = {(*(vertBuffer + i)).Position, 
		//		(*(vertBuffer + i)).UV, (*(vertBuffer + i)).Normal}; 
		//		vertData.push_back(nomorph); 
		// 
		//		myMesh = std::make_shared<Mesh>(indices, indices.size(), 
		//			vertData, vertData.size()); 
		//		meshList.push_back(myMesh); 
		//	} 
		//} 
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

		//vertex data
		std::vector<uint32_t> indices;
		std::vector<Vertex> vertData;
		std::vector<MorphVertex> morphVertData;

		//tangents
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;

		//Math::computeTangents(morphVertData, indices, tangents, bitangents);
		for (int i = 0; i < vertTotalBuffer; i++) {
			MorphVertex morph = { Vertex{(*(morphBuffer + i)).Position,
			(*(morphBuffer + i)).UV, (*(morphBuffer + i)).Normal}, (*(morphBuffer + i)).PositionS, 
				(*(morphBuffer + i)).NormalS };
			morphVertData.push_back(morph);
		}


		for (int i = 0; i < indiceTotalBuffer; i++) {
			indices.push_back(*(indicesBuffer + i));
		}

		myMesh = std::make_shared<Mesh>(indices, indices.size(),
			morphVertData, morphVertData.size());
		meshList.push_back(myMesh);
	}

	return true;
}

