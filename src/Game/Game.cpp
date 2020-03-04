#include "Game.h" 
#include "Graphics/Object Graphics/MeshRenderer.h"
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

void GlfwWindowResizedCallback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
	//Vatista::Window* gameWindow = ; 
	//if (gameWindow != nullptr) { 
	//	gameWindow->getWindow()->resize(width, height); 
	//} 
}

void Vatista::Game::run()
{
	init();

	static float previousFrame = (float)glfwGetTime();

	//game loop 
	while (!gameWindow->shouldClose()) {
		static float currentFrame = (float)glfwGetTime();
		static float deltaTime = currentFrame - previousFrame;

		update(deltaTime);
		render(deltaTime);
		//draw(deltaTime);

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
	mainCamera->SetPosition(glm::vec3(0.0f, 2.0f, 10.0f));
	mainCamera->LookAt(glm::vec3(0.0f, 2.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mainCamera->Projection = glm::perspective(glm::radians(60.0f), 16.f / 9.f, 1.0f, 200.0f);

	orthoCamera = std::make_shared<Vatista::Camera>();
	orthoCamera->SetPosition(glm::vec3(0.0f));
	orthoCamera->LookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	orthoCamera->Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

	//audio
	audioEngine = std::make_shared<AudioEngine>();
	audioEngine->Init();
	audioEngine->LoadBank("./res/Music and Sound/Master", FMOD_STUDIO_LOAD_BANK_NORMAL);
	audioEngine->LoadEvent("Music", "{fd7618a1-5880-4a64-ae37-e0f01ef54912}");
	audioEngine->LoadEvent("LightAttack", "{f64fa79a-565d-4493-b4bf-a73a37c31219}");
	audioEngine->LoadEvent("HeavyAttack", "{621b2e70-27ea-4900-b397-96cb10366574}");
	audioEngine->LoadEvent("Dash", "{53dbc862-3dec-411a-9fc4-bb15743c2b6b}");
	audioEngine->PlayEvent("Music");
	
	load("./res/Objects/init.txt");


	textureStamina = std::make_shared<Texture2D>();//connected to staminamat
	textureStamina->loadFile("./res/Objects/Stamina/staminaRampTexture.png");

	texture = std::make_shared<Texture2D>();
	texture->loadFile("./res/Objects/Z3n/Z3N_Texture.png");

	//texture3 = std::make_shared<Texture2D>();
	//texture3->loadFile("./res/default.png");

	Shader::Sptr phong = std::make_shared<Shader>();
	phong->Load("./res/Shaders/passthroughMorph.vs", "./res/Shaders/blinn-phong.fs.glsl");

	Shader::Sptr phong2 = std::make_shared<Shader>();
	phong2->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/blinn-phong.fs.glsl");

	Shader::Sptr staminaPhong = std::make_shared<Shader>();//stamina stuff
	staminaPhong->Load("./res/Shaders/StaminaBar.vs.glsl", "./res/Shaders/StaminaBar.fs.glsl");

	Shader::Sptr staminaRamp = std::make_shared<Shader>();//stamina stuff
	staminaRamp->Load("./res/Shaders/StaminaBar.vs.glsl", "./res/Shaders/StaminaBar.fs.glsl");

	//sample testing
	TextureSampler::Sptr NearestMipped = std::make_shared<TextureSampler>();
	NearestMipped->minFilter = MinFilter::NearestMipNearest;
	NearestMipped->magFilter = MagFilter::Nearest;
	NearestMipped->createSampler();

	Material::Sptr testMat = std::make_shared<Material>(phong);
	testMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	testMat->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
	testMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	testMat->Set("a_AmbientPower", 0.5f);
	testMat->Set("a_LightSpecPower", 0.9f);
	testMat->Set("a_LightShininess", 256.0f);
	testMat->Set("a_LightAttenuation", 0.04f);
	testMat->Set("texSample", texture, NearestMipped);

	Material::Sptr testMat2 = std::make_shared<Material>(phong2);
	testMat2->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	testMat2->Set("a_LightColor", { 0.0f, 1.0f, 0 });
	testMat2->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	testMat2->Set("a_AmbientPower", 0.5f);
	testMat2->Set("a_LightSpecPower", 0.9f);
	testMat2->Set("a_LightShininess", 256.0f);
	testMat2->Set("a_LightAttenuation", 0.04f);
	testMat2->Set("texSample", texture2, NearestMipped);

	//load textures
	for (int i = 0; i < 17; i++) {
		textures.emplace_back();
		textures[i] = std::make_shared<Texture2D>();
		switch (i) {
		case 0:
			textures[i]->loadFile("./res/Objects/Cave/Cave_Texture.png");
			break;
		case 1:
			textures[i]->loadFile("./res/Objects/Background_Objects/Background_Objects_Texture.png");
			break;
		case 2:
			textures[i]->loadFile("./res/Objects/Bunker/Bunker_Texture.png");
			break;
		case 3:
			textures[i]->loadFile("./res/Objects/Bunker_Door/Bunker_Door_Texture.png");
			break;
		case 4:
			textures[i]->loadFile("./res/Objects/Drill/Drill_Texture.png");
			break;
		case 5:
			textures[i]->loadFile("./res/Objects/Catwalk_Floor/Catwalk_Floor_Texture.png");
			break;
		case 6:
			textures[i]->loadFile("./res/Objects/Catwalk_InsideCorner/Catwalk_Rail_Corners_Texture.png");
			break;
		case 7:
			textures[i]->loadFile("./res/Objects/Catwalk_OutsideCorner/Catwalk_Rail_Corners_Texture.png");
			break;
		case 8:
			textures[i]->loadFile("./res/Objects/Catwalk_Rail_Large/Catwalk_Rail_Large_Texture.png");
			break;
		case 9:
			textures[i]->loadFile("./res/Objects/Catwalk_Rail_Short/Catwalk_Rail_Short_Texture.png");
			break;
		case 10:
			textures[i]->loadFile("./res/Objects/CrateLarge/CrateLarge_Texture.png");
			break;
		case 11:
			textures[i]->loadFile("./res/Objects/CrateMedium/CrateMedium_Texture.png");
			break;
		case 12:
			textures[i]->loadFile("./res/Objects/CrateSmall/CrateSmall_Texture.png");
			break;
		case 13:
			textures[i]->loadFile("./res/Objects/Girders/Girders_Texture.png");
			break;
		case 14:
			textures[i]->loadFile("./res/Objects/MiningShip/MiningShip_Texture.png");
			break;
		case 15:
			textures[i]->loadFile("./res/Objects/Barrel/Barrel_Texture.png");
			break;
		case 16:
			textures[i]->loadFile("./res/Objects/Electrical_Stuff/Wires_&_Lights_Texture.png");
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
		mats[i]->Set("texSample", textures[i], NearestMipped);
	}
	

	for (int i = 0; i < 17; i++) {
		stage = std::make_shared<StationaryObj>();
		stage->setPos(glm::vec3(0.0f, 0.0f, 5.0f));
		stage->setRotY(-90.0f);
		stage->setMesh(meshList[i+1]);
		stage->setMat(mats[i]);
		ObjectList.push_back(stage);
	}

	//stamina
	Material::Sptr staminaMat = std::make_shared<Material>(staminaPhong);//blank stamina texture
	staminaMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	staminaMat->Set("a_LightColor", { 0.0f, 1.0f, 0 });
	staminaMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	staminaMat->Set("a_AmbientPower", 0.5f);
	staminaMat->Set("a_LightSpecPower", 0.9f);
	staminaMat->Set("a_LightShininess", 256.0f);
	staminaMat->Set("a_LightAttenuation", 0.04f);
	staminaMat->Set("texSample", textureStamina, NearestMipped);
	staminaMat->Set("UVoffset", glm::vec3(0.0f));


	//Player 1
	C1 = std::make_shared<Character>(true, meshList[0], testMat);
	ObjectList.push_back(C1);

	//Player 2 
	C2 = std::make_shared<Character>(false, meshList[0], testMat);
	ObjectList.push_back(C2);


	TestStamina = std::make_shared<Stamina>();
	TestStamina->setPos(glm::vec3(-5.f, 7.f, 0.f));
	TestStamina->setMesh(meshList[18]);//3rd one on init.txt 
	TestStamina->setMat(staminaMat/*staminaMat*/);

	TestStamina->setRot(glm::vec3(90.f, 0.f, 0.f));
	//TestStamina->setTexture(textureStamina/*placeholder*/);//might want to use fbo rended texture to change it in real time 
	TestStamina->setScale(glm::vec3(0.5f));
	UIList.push_back(TestStamina);

	TestStamina2 = std::make_shared<Stamina>();
	TestStamina2->setPos(glm::vec3(5.f, 7.f, 0.f));
	TestStamina2->setMesh(meshList[18]);//3rd one on init.txt 
	TestStamina2->setMat(staminaMat/*staminaMat*/);

	TestStamina2->setRot(glm::vec3(90.f, 0.f, 0.f));
	//TestStamina2->setTexture(textureStamina/*placeholder*/);//might want to use fbo rended texture to change it in real time 
	TestStamina2->setScale(glm::vec3(0.5f));
	UIList.push_back(TestStamina2);
	 

	glEnable(GL_CULL_FACE);
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

	C1->setStamina(C1->getStamina() - 0.0005f);

	TestStamina->setStamina(C1->getStamina());
	TestStamina2->setStamina(C2->getStamina());
	float dist = fabs(C1->getPosX() - C2->getPosX());
	//C1->setStamina(C1->getStamina() + 10.0f);
	//C2->setStamina(C2->getStamina() + 10.0f);
	if (dist > 5.0f)
		mainCamera->SetPosition(glm::vec3((C1->getPosX() + C2->getPosX()) / 2.0f, 2.0f, 11.0f + (dist / 2.0f)));
	else
		mainCamera->SetPosition(glm::vec3((C1->getPosX() + C2->getPosX()) / 2.0f, 2.0f, 13.5f));
	mainCamera->LookAt(glm::vec3(0.0f, 2.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	audioEngine->Update();

	//TestStamina.setScale(glm::vec3(0.5f)); 
}

void Vatista::Game::render(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	draw(dt);
}

void Vatista::Game::draw(float)
{

	//draw game objects
	for (auto object : ObjectList) {
		object->Draw(mainCamera);
	}

	//draw UI
	for (auto component : UIList) {
		component->Draw(orthoCamera);
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
		dataFile.open("./res/Objects/" + dataList[i], std::ios::in | std::ios::binary);
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

		//vertex data
		std::vector<uint32_t> indices;
		std::vector<Vertex> vertData;
		std::vector<MorphVertex> morphVertData;

		//tangents
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;

		//Math::computeTangents(morphVertData, indices, tangents, bitangents);
		for (int j = 0; j < vertTotalBuffer; j++) {
			MorphVertex morph = { Vertex{(*(morphBuffer + j)).Position,
			(*(morphBuffer + j)).UV, (*(morphBuffer + j)).Normal}, (*(morphBuffer + j)).PositionS, 
				(*(morphBuffer + j)).NormalS };
			morphVertData.push_back(morph);
		}


		for (int k = 0; k < indiceTotalBuffer; k++) {
			indices.push_back(*(indicesBuffer + k));
		}

		myMesh = std::make_shared<Mesh>(indices, indices.size(),
			morphVertData, morphVertData.size());
		meshList.push_back(myMesh);
	}

	return true;
}

