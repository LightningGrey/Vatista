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
	gameWindow = new Vatista::Window(1600, 900, "Alpha Strike");

	mainCamera = std::make_shared<Vatista::Camera>();
	mainCamera->SetPosition(glm::vec3(0, 4, 15));
	mainCamera->LookAt(glm::vec3(0,4,0), glm::vec3(0, 1, 0));
	mainCamera->Projection = glm::perspective(glm::radians(60.0f), 16.f / 9.f, 1.0f, 200.0f);

	//doesn't work yet
	orthoCamera = std::make_shared<Vatista::Camera>();
	orthoCamera->SetPosition(glm::vec3(0, 0, 15));
	orthoCamera->LookAt(glm::vec3(0), glm::vec3(0, 1, 0));
	orthoCamera->Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 150.0f);

	//audio
	audioEngine = std::make_shared<AudioEngine>();
	audioEngine->Init();
	audioEngine->LoadBank("./res/Master", FMOD_STUDIO_LOAD_BANK_NORMAL);
	audioEngine->LoadEvent("Music", "{fd7618a1-5880-4a64-ae37-e0f01ef54912}");
	audioEngine->LoadEvent("LightAttack", "{f64fa79a-565d-4493-b4bf-a73a37c31219}");
	audioEngine->LoadEvent("HeavyAttack", "{621b2e70-27ea-4900-b397-96cb10366574}");
	audioEngine->LoadEvent("Dash", "{53dbc862-3dec-411a-9fc4-bb15743c2b6b}");
	audioEngine->PlayEvent("Music");
	load("./res/init.txt");
	// don't delete its gonna be the ui camera later
	//myCameraUI = std::make_shared<Vatista::Camera>();//its the ui camera but we already have it orthagraphic by default -gary
	//myCameraUI->SetPosition(glm::vec3(0, 0, 1));
	//myCameraUI->LookAt(glm::vec3(0), glm::vec3(0, 1, 0));
	//myCameraUI->Projection = glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, 0.f, 1000.0f);

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

	textureStamina = std::make_shared<Texture>();//connected to staminamat
	textureStamina->loadFile("./res/staminaRampTexture.png");

	texture = std::make_shared<Texture>();
	texture->loadFile("./res/Z3N_Texture.png");

	texture2 = std::make_shared<Texture>();
	texture2->loadFile("./res/Catwalk_Floor/Catwalk_Floor_Texture.png");

	texture3 = std::make_shared<Texture>();
	texture3->loadFile("./res/default.png");

	Shader::Sptr phong = std::make_shared<Shader>();
	phong->Load("./res/passthroughMorph.vs", "./res/blinn-phong.fs.glsl");

	Shader::Sptr phong2 = std::make_shared<Shader>();
	phong2->Load("./res/lighting.vs.glsl", "./res/blinn-phong.fs.glsl");

	Shader::Sptr staminaPhong = std::make_shared<Shader>();//stamina stuff
	staminaPhong->Load("./res/StaminaBar.vs.glsl", "./res/StaminaBar.fs.glsl");

	Shader::Sptr staminaRamp = std::make_shared<Shader>();//stamina stuff
	staminaRamp->Load("./res/StaminaBar.vs.glsl", "./res/StaminaBar.fs.glsl");


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

	for (int i = 0; i < 17; i++) {
		textures.emplace_back();
		textures[i] = std::make_shared<Texture>();
		switch (i) {
		case 0:
			textures[i]->loadFile("./res/Cave/Cave_Texture.png");
			break;
		case 1:
			textures[i]->loadFile("./res/Background_Objects/Background_Objects_Texture.png");
			break;
		case 2:
			textures[i]->loadFile("./res/Bunker/Bunker_Texture.png");
			break;
		case 3:
			textures[i]->loadFile("./res/Bunker_Door/Bunker_Door_Texture.png");
			break;
		case 4:
			textures[i]->loadFile("./res/Drill_Texture.png");
			break;
		case 5:
			textures[i]->loadFile("./res/Catwalk_Floor/Catwalk_Floor_Texture.png");
			break;
		case 6:
			textures[i]->loadFile("./res/Catwalk_InsideCorner/Catwalk_Rail_Corners_Texture.png");
			break;
		case 7:
			textures[i]->loadFile("./res/Catwalk_OutsideCorner/Catwalk_Rail_Corners_Texture.png");
			break;
		case 8:
			textures[i]->loadFile("./res/Catwalk_Rail_Large/Catwalk_Rail_Large_Texture.png");
			break;
		case 9:
			textures[i]->loadFile("./res/Catwalk_Rail_Short/Catwalk_Rail_Short_Texture.png");
			break;
		case 10:
			textures[i]->loadFile("./res/CrateLarge/CrateLarge_Texture.png");
			break;
		case 11:
			textures[i]->loadFile("./res/CrateMedium/CrateMedium_Texture.png");
			break;
		case 12:
			textures[i]->loadFile("./res/CrateSmall/CrateSmall_Texture.png");
			break;
		case 13:
			textures[i]->loadFile("./res/Girders/Girders_Texture.png");
			break;
		case 14:
			textures[i]->loadFile("./res/MiningShip/MiningShip_Texture.png");
			break;
		case 15:
			textures[i]->loadFile("./res/Barrel/Barrel_Texture.png");
			break;
		case 16:
			textures[i]->loadFile("./res/Electrical_Stuff/Wires_&_Lights_Texture.png");
			break;
		}
	}

	for (int i = 0; i < 17; i++) {
		mats.emplace_back();
		mats[i] = std::make_shared<Material>(phong2);
		mats[i]->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
		mats[i]->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
		mats[i]->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
		mats[i]->Set("a_AmbientPower", 0.5f);
		mats[i]->Set("a_LightSpecPower", 0.9f);
		mats[i]->Set("a_LightShininess", 256.0f);
		mats[i]->Set("a_LightAttenuation", 0.04f);
		mats[i]->Set("texSample", textures[i]);
	}
	

	for (int i = 0; i < 17; i++) {
		stage = std::make_shared<GameObject>();
		stage->setPos(glm::vec3(0, 0, 5));
		stage->setRotY(-90.0f);
		stage->setMesh(meshList[i+1]);
		stage->setMat(mats[i]);
		ObjectList.push_back(stage);
	}

	Material::Sptr staminaMat = std::make_shared<Material>(staminaPhong);//blank stamina texture
	staminaMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	staminaMat->Set("a_LightColor", { 0.0f, 1.0f, 0 });
	staminaMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	staminaMat->Set("a_AmbientPower", 0.5f);
	staminaMat->Set("a_LightSpecPower", 0.9f);
	staminaMat->Set("a_LightShininess", 256.0f);
	staminaMat->Set("a_LightAttenuation", 0.04f);
	staminaMat->Set("texSample", textureStamina);
	staminaMat->Set("UVoffset", glm::vec3(0.0f));

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
	//TestMap = std::make_shared<GameObject>();
	//TestMap->setPos(glm::vec3(0, 0.f, 5));
	//TestMap->setMesh(meshList[1]);
	//TestMap->setMat(testMat2);
	//TestMap->setRotY(-90.f);
	//TestMap->setTexture(texture2);
	//TestMap->setScale(glm::vec3(0.4f));
	//ObjectList.push_back(TestMap);
	
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


	TestStamina = std::make_shared<GameObject>();
	TestStamina->setPos(glm::vec3(-5, 10, 3));
	TestStamina->setMesh(meshList[18]);//3rd one on init.txt
	TestStamina->setMat(staminaMat/*staminaMat*/);

	TestStamina->setRot(glm::vec3(90, 0, 0));
	TestStamina->setTexture(textureStamina/*placeholder*/);//might want to use fbo rended texture to change it in real time
	TestStamina->setScale(glm::vec3(0.5));
	UIList.push_back(TestStamina);

	TestStamina2 = std::make_shared<GameObject>();
	TestStamina2->setPos(glm::vec3(5, 10, 3));
	TestStamina2->setMesh(meshList[18]);//3rd one on init.txt
	TestStamina2->setMat(staminaMat/*staminaMat*/);

	TestStamina2->setRot(glm::vec3(90, 0, 0));
	TestStamina2->setTexture(textureStamina/*placeholder*/);//might want to use fbo rended texture to change it in real time
	TestStamina2->setScale(glm::vec3(0.5));
	UIList.push_back(TestStamina2);
	 



	//TestStaminaBackground = std::make_shared<GameObject>();
	//TestStaminaBackground->setPos(glm::vec3(1, -1.f, 1));
	//TestStaminaBackground->setMesh(meshList[6/*placeholder*/]);//3rd one on init.txt
	//TestStaminaBackground->setMat(testMat2/*placeholder*/);
	//TestStaminaBackground->setRotY(-90.f);
	//TestStaminaBackground->setTexture(texture2/*placeholder*/);//might want to use fbo rended texture to change it in real time
	//TestStaminaBackground->setScale(glm::vec3(1.0f));
	//ObjectList.push_back(TestStaminaBackground);


	glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
}



void Vatista::Game::close()
{
	glfwTerminate();
	audioEngine->Shutdown();
}

void Vatista::Game::update(float dt)
{
	C1->update(dt, gameWindow->getWindow(), C2, audioEngine);
	C2->update(dt, gameWindow->getWindow(), C1, audioEngine);

	TestStamina->setStamina(C1->getStamina());
	TestStamina2->setStamina(C2->getStamina());

	//delete later
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_ADD)) {
		x += 0.01f;
		x = x > 1.0f ? 1.0f : x;
		TestStamina->setStamina(C1->getStamina() + 0.01);
	} 

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_SUBTRACT)){
		x -= 0.01f;
		x = x < 0.0f ? 0.0f : x;
		TestStamina->setScale(glm::vec3(x, 1.0f, 1.0f));
	}

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_8)){
		y += 0.01f;
		y = y > 1.0f ? 1.0f : y;
		TestStamina2->setScale(glm::vec3(y, 1.0f, 1.0f));
	}

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_2)) {
		y -= 0.01f;
		y = y < 0.0f ? 0.0f : y;
		TestStamina2->setScale(glm::vec3(y, 1.0f, 1.0f));
	}


	audioEngine->Update();

	//TestStamina.setScale(glm::vec3(0.5f));
}

void Vatista::Game::draw(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	//draw 
	for (auto object : ObjectList) {
		object->Draw(mainCamera);
	}

	for (auto component : UIList) {
		component->Draw(mainCamera);
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

