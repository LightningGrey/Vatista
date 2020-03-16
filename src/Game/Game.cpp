#include "Game.h" 
#include "Graphics/Rendering/MeshRenderer.h"
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
	gameWindow = new Vatista::Window(1600, 900, "Zeal: 2341");

	mainCamera = std::make_shared<Vatista::Camera>();
	mainCamera->SetPosition(glm::vec3(0.0f, 2.0f, 10.0f));
	mainCamera->LookAt(glm::vec3(0.0f, 2.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mainCamera->Projection = glm::perspective(glm::radians(60.0f), 16.f / 9.f, 1.0f, 500.0f);

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

	texture = std::make_shared<Texture2D>();
	texture->loadFile("./res/Objects/Z3n/Z3N_Texture.png");

	texture2 = std::make_shared<Texture2D>();
	texture2->loadFile("./res/Objects/color-grid.png");

	Shader::Sptr character = std::make_shared<Shader>();
	character->Load("./res/Shaders/passthroughMorph.vs", "./res/Shaders/blinn-phong.fs.glsl");

	Shader::Sptr stageProp = std::make_shared<Shader>();
	stageProp->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/blinn-phong.fs.glsl");

	//Shader::Sptr staminaUIPhong = std::make_shared<Shader>();
	//staminaUIPhong->Load("./res/Shaders/passthroughMorph.vs", "./res/Shaders/blinn-phong.fs.glsl");

	Shader::Sptr staminaPhong = std::make_shared<Shader>();//stamina stuff
	staminaPhong->Load("./res/Shaders/StaminaBar.vs.glsl", "./res/Shaders/StaminaBar.fs.glsl");

	Shader::Sptr staminaRamp = std::make_shared<Shader>();//stamina stuff
	staminaRamp->Load("./res/Shaders/StaminaBar.vs.glsl", "./res/Shaders/StaminaBar.fs.glsl");

	//sample testing
	TextureSampler::Sptr NearestMipped = std::make_shared<TextureSampler>();
	NearestMipped->minFilter = MinFilter::NearestMipNearest;
	NearestMipped->magFilter = MagFilter::Nearest;
	NearestMipped->createSampler();

	Material::Sptr characterMat = std::make_shared<Material>(character);
	characterMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	characterMat->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
	characterMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	characterMat->Set("a_AmbientPower", 0.7f);
	characterMat->Set("a_LightSpecPower", 0.9f);
	characterMat->Set("a_LightShininess", 256.0f);
	characterMat->Set("a_LightAttenuation", 0.04f);
	characterMat->Set("texSample", texture, NearestMipped);
	
	Material::Sptr stageMat = std::make_shared<Material>(stageProp);
	stageMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	stageMat->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
	stageMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	stageMat->Set("a_AmbientPower", 0.5f);
	stageMat->Set("a_LightSpecPower", 0.9f);
	stageMat->Set("a_LightShininess", 256.0f);
	stageMat->Set("a_LightAttenuation", 0.04f);
	stageMat->Set("texSample", texture2, NearestMipped);


	//======= STAGE ONE =======

	for (int i = 0; i < 17; i++) {
		textures = std::make_shared<Texture2D>();
		switch (i) {
		case 0:
			textures->loadFile("./res/Objects/Cave/Cave_Texture.png");
			break;
		case 1:
			textures->loadFile("./res/Objects/Background_Objects/Background_Objects_Texture.png");
			break;
		case 2:
			textures->loadFile("./res/Objects/Bunker/Bunker_Texture.png");
			break;
		case 3:
			textures->loadFile("./res/Objects/Bunker_Door/Bunker_Door_Texture.png");
			break;
		case 4:
			textures->loadFile("./res/Objects/Drill/Drill_Texture.png");
			break;
		case 5:
			textures->loadFile("./res/Objects/Catwalk_Floor/Catwalk_Floor_Texture.png");
			break;
		case 6:
			textures->loadFile("./res/Objects/Catwalk_InsideCorner/Catwalk_Rail_Corners_Texture.png");
			break;
		case 7:
			textures->loadFile("./res/Objects/Catwalk_OutsideCorner/Catwalk_Rail_Corners_Texture.png");
			break;
		case 8:
			textures->loadFile("./res/Objects/Catwalk_Rail_Large/Catwalk_Rail_Large_Texture.png");
			break;
		case 9:
			textures->loadFile("./res/Objects/Catwalk_Rail_Short/Catwalk_Rail_Short_Texture.png");
			break;
		case 10:
			textures->loadFile("./res/Objects/CrateLarge/CrateLarge_Texture.png");
			break;
		case 11:
			textures->loadFile("./res/Objects/CrateMedium/CrateMedium_Texture.png");
			break;
		case 12:
			textures->loadFile("./res/Objects/CrateSmall/CrateSmall_Texture.png");
			break;
		case 13:
			textures->loadFile("./res/Objects/Girders/Girders_Texture.png");
			break;
		case 14:
			textures->loadFile("./res/Objects/MiningShip/MiningShip_Texture.png");
			break;
		case 15:
			textures->loadFile("./res/Objects/Barrel/Barrel_Texture.png");
			break;
		case 16:
			textures->loadFile("./res/Objects/Electrical_Stuff/Wires_&_Lights_Texture.png");
			break;
		}
		mats = std::make_shared<Material>(stageProp);
		if (i == 5) {
			mats->Set("a_LightPos", { -4.0f, 1.0f, 5.0f });
		}
		else {
			mats->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
		}

		if (i == 5) {
			mats->Set("a_LightColor", { 0.5f, 0.5f, 0.5f });
		}
		else {
			mats->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
		}

		mats->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
		if (i == 5) {
			mats->Set("a_AmbientPower", 0.8f);
		}
		else {
			mats->Set("a_AmbientPower", 0.25f);
		}
		mats->Set("a_LightSpecPower", 0.9f);
		mats->Set("a_LightShininess", 256.0f);
		if (i == 5) {
			mats->Set("a_LightAttenuation", 0.04f);
		}
		else {
			mats->Set("a_LightAttenuation", 0.04f);
		}
		mats->Set("texSample", textures, NearestMipped);
		stage = std::make_shared<StationaryObj>();
		stage->setPos(glm::vec3(0.0f, 0.0f, 5.0f));
		stage->setRotY(-90.0f);
		stage->setMesh(meshList[i]);
		stage->setMat(mats);
		//if (i != 5) {
			ObjectList.push_back(stage);
		//}
	}

	//// ======= STAGE TWO =======
	//
	//for (int i = 52; i < 55; i++) {
	//	textures = std::make_shared<Texture2D>();
	//	switch (i) {
	//	case 52:
	//		textures->loadFile("./res/Objects/Terminal/Background_Texture.png");
	//		break;
	//	case 53:
	//		textures->loadFile("./res/Objects/Terminal/Vatista_Terminal_Texture.png");
	//		break;
	//	case 54:
	//		textures->loadFile("./res/Objects/Terminal/CargoShip_Texture.png");
	//		break;
	//	}
	//	mats = std::make_shared<Material>(stageProp);
	//	mats->Set("a_LightPos", { 0.0f, 3.0f, 8.0f });
	//	mats->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
	//	mats->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	//	mats->Set("a_AmbientPower", 0.5f);
	//	mats->Set("a_LightSpecPower", 0.9f);
	//	mats->Set("a_LightShininess", 256.0f);
	//	mats->Set("a_LightAttenuation", 0.25f);
	//	mats->Set("texSample", textures, NearestMipped);
	//	mats->Set("rimOn", 1);
	//	stage = std::make_shared<StationaryObj>();
	//	stage->setPos(glm::vec3(0.0f, 0.0f, 5.0f));
	//	stage->setRotY(-90.0f);
	//	stage->setMesh(meshList[i]);
	//	stage->setMat(mats);
	//	ObjectList.push_back(stage);
	//}
	//
	//

	for (int i = 0; i < 4; i++) {
		stamUIText = std::make_shared<Texture2D>();
		switch (i) {
		case 0:
			stamUIText->loadFile("./res/Objects/UI/ui_hp0.png");
			break;
		case 1:
			stamUIText->loadFile("./res/Objects/UI/ui_hp1.png");
			break;
		case 2:
			stamUIText->loadFile("./res/Objects/UI/ui_hp2.png");
			break;
		case 3:
			stamUIText->loadFile("./res/Objects/UI/ui_hp3.png");
			break;
		}
		stamUIMats.emplace_back();
		stamUIMats[i] = std::make_shared<Material>(stageProp);
		stamUIMats[i]->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
		stamUIMats[i]->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
		stamUIMats[i]->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
		stamUIMats[i]->Set("a_AmbientPower", 0.5f);
		stamUIMats[i]->Set("a_LightSpecPower", 0.9f);
		stamUIMats[i]->Set("a_LightShininess", 256.0f);
		stamUIMats[i]->Set("a_LightAttenuation", 0.04f);
		stamUIMats[i]->Set("texSample", stamUIText, NearestMipped);
	}
	textureStamina = std::make_shared<Texture2D>();//connected to staminamat
	textureStamina->loadFile("./res/Objects/Stamina/staminaRampTexture1.png");

	stamUIText = std::make_shared<Texture2D>();
	stamUIText->loadFile("./res/Objects/Z3n/Z3n_render.png");

	Material::Sptr profileMat = std::make_shared<Material>(stageProp);
	profileMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	profileMat->Set("a_LightColor", { 0.0f, 1.0f, 0.0f });
	profileMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	profileMat->Set("a_AmbientPower", 0.5f);
	profileMat->Set("a_LightSpecPower", 0.9f);
	profileMat->Set("a_LightShininess", 256.0f);
	profileMat->Set("a_LightAttenuation", 0.04f);
	profileMat->Set("texSample", stamUIText, NearestMipped);

	//stamina
	Material::Sptr staminaMat = std::make_shared<Material>(staminaPhong);//blank stamina texture
	staminaMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	staminaMat->Set("a_LightColor", { 0.0f, 1.0f, 0.0f });
	staminaMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	staminaMat->Set("a_AmbientPower", 0.5f);
	staminaMat->Set("a_LightSpecPower", 0.9f);
	staminaMat->Set("a_LightShininess", 256.0f);
	staminaMat->Set("a_LightAttenuation", 0.04f);
	staminaMat->Set("texSample", textureStamina, NearestMipped);
	staminaMat->Set("UVoffset", glm::vec3(0.0f));

	//Player 1
	C1 = std::make_shared<Character>(true, meshList, characterMat);
	C1->setScale(0.01f);
	ObjectList.push_back(C1);

	//Player 2 
	C2 = std::make_shared<Character>(false, meshList, characterMat);
	C2->setScale(0.01f);
	ObjectList.push_back(C2);

	bladeText = std::make_shared<Texture2D>();
	bladeText->loadFile("./res/Objects/color-grid.png");
	Material::Sptr bladeMat = std::make_shared<Material>(stageProp);//blank stamina texture
	bladeMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	bladeMat->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
	bladeMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	bladeMat->Set("a_AmbientPower", 0.5f);
	bladeMat->Set("a_LightSpecPower", 0.9f);
	bladeMat->Set("a_LightShininess", 256.0f);
	bladeMat->Set("a_LightAttenuation", 0.04f);
	bladeMat->Set("texSample", bladeText, NearestMipped);

	sword1 = std::make_shared<StationaryObj>();
	sword1->setMesh(meshList[50]);
	sword1->setMat(bladeMat);
	sword1->setPos(C1->getPos());
	sword1->setScale(0.01f);
	ObjectList.push_back(sword1);
	sword2 = std::make_shared<StationaryObj>();
	sword2->setMesh(meshList[50]);
	sword2->setMat(bladeMat);
	sword2->setPos(C2->getPos());
	sword2->setScale(0.01f);
	ObjectList.push_back(sword2);
	bladeText = std::make_shared<Texture2D>();
	bladeText->loadFile("./res/Objects/Z3n/z3n_Sheath_Texture.png");
	bladeMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	bladeMat->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
	bladeMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	bladeMat->Set("a_AmbientPower", 0.5f);
	bladeMat->Set("a_LightSpecPower", 0.9f);
	bladeMat->Set("a_LightShininess", 256.0f);
	bladeMat->Set("a_LightAttenuation", 0.04f);
	bladeMat->Set("texSample", bladeText, NearestMipped);

	sheath1 = std::make_shared<StationaryObj>();
	sheath1->setMesh(meshList[51]);
	sheath1->setMat(bladeMat);
	sheath1->setPos(C1->getPos());
	sheath1->setScale(0.01f);
	ObjectList.push_back(sheath1);
	sheath2 = std::make_shared<StationaryObj>();
	sheath2->setMesh(meshList[51]);
	sheath2->setMat(bladeMat);
	sheath2->setPos(C2->getPos());
	sheath2->setScale(0.01f);
	ObjectList.push_back(sheath2);

	UI1 = std::make_shared<UIObject>();
	UI1->setPos(glm::vec3(-4.8f, 7.f, 0.f));
	UI1->setMesh(meshList[17]);
	UI1->setMat(stamUIMats[C1->getLives()]);
	UI1->setScale(2.5f);
	UIList.push_back(UI1);

	charProfile1 = std::make_shared<UIObject>();
	charProfile1->setPos(glm::vec3(-4.8f, 7.f, 1.f));
	charProfile1->setMesh(meshList[18]);
	charProfile1->setMat(profileMat);
	charProfile1->setScale(2.5f);
	UIList.push_back(charProfile1);

	S1 = std::make_shared<Stamina>();
	S1->setPos(glm::vec3(-4.8f, 7.f, 1.f));
	S1->setMesh(meshList[19]);//3rd one on init.txt 
	S1->setMat(staminaMat/*staminaMat*/);
	//S1->setTexture(textureStamina/*placeholder*/);//might want to use fbo rended texture to change it in real time 
	S1->setScale(2.5f);
	UIList.push_back(S1);


	UI2 = std::make_shared<UIObject>();
	UI2->setPos(glm::vec3(4.8f, 7.f, 0.f));
	UI2->setMesh(meshList[17]);
	UI2->setMat(stamUIMats[C2->getLives()]);

	UI2->setRotY(180.f);
	UI2->setScale(2.5f);
	UIList.push_back(UI2);

	charProfile2 = std::make_shared<UIObject>();
	charProfile2->setPos(glm::vec3(4.8f, 7.f, 1.f));
	charProfile2->setMesh(meshList[18]);
	charProfile2->setMat(profileMat);
	charProfile2->setRotY(180.f);
	charProfile2->setScale(2.5f);
	UIList.push_back(charProfile2);

	S2 = std::make_shared<Stamina>();
	S2->setPos(glm::vec3(4.8f, 7.f, 1.f));
	S2->setMesh(meshList[19]);//3rd one on init.txt 
	S2->setMat(staminaMat/*staminaMat*/);
	S2->setRotY(180.f);
	//S2->setTexture(textureStamina/*placeholder*/);//might want to use fbo rended texture to change it in real time 
	S2->setScale(2.5f);
	UIList.push_back(S2);

	//bufferCreation();
	 

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
	if (C1->getLives() > 0 && C2->getLives() > 0) {
		C1->update(dt, gameWindow->getWindow(), C2, audioEngine);
		C2->update(dt, gameWindow->getWindow(), C1, audioEngine);
		sword1->setPos(C1->getPos());
		sword1->setRotY(C1->getRot().y);
		sheath1->setPos(C1->getPos());
		sheath1->setRotY(C1->getRot().y);
		sword2->setPos(C2->getPos());
		sword2->setRotY(C2->getRot().y);
		sheath2->setPos(C2->getPos());
		sheath2->setRotY(C2->getRot().y);
		UI1->setMat(stamUIMats[C1->getLives()]);
		UI2->setMat(stamUIMats[C2->getLives()]);
		S1->setStamina(C1->getStamina());
		S2->setStamina(C2->getStamina());
		std::cout << C1->getStamina() << " " << C2->getStamina() << std::endl;
		float dist = fabs(C1->getPosX() - C2->getPosX());
		//C1->setStamina(C1->getStamina() + 10.0f);
		//C2->setStamina(C2->getStamina() + 10.0f);
		if (dist > 5.0f)
			mainCamera->SetPosition(glm::vec3((C1->getPosX() + C2->getPosX()) / 2.0f, 2.0f, 11.0f + (dist / 2.0f)));
		else
			mainCamera->SetPosition(glm::vec3((C1->getPosX() + C2->getPosX()) / 2.0f, 2.0f, 13.5f));
		mainCamera->LookAt(glm::vec3(0.0f, 2.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if(C1->getLives()==0){

	}
	else if(C2->getLives()==0){

	}
	
	audioEngine->Update();

	//S1.setScale(glm::vec3(0.5f)); 

	//Shader::Sptr lighting = std::make_shared<Shader>();
	//lighting->Bind();
	//lighting->SetUniform("light.ambient", 0.0f);
	//lighting->Compile();

}

void Vatista::Game::render(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

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

void Vatista::Game::bufferCreation()
{
	RenderBufferDesc mainColour = RenderBufferDesc();
	mainColour.ShaderReadable = true;
	mainColour.Attachment = RenderTargetAttachment::Color0;
	mainColour.Format = RenderTargetType::ColorRgb8; //RGB8

	//RenderBufferDesc normal = RenderBufferDesc();
	//normal.ShaderReadable = true;
	//normal.Attachment = RenderTargetAttachment::Color1;
	//normal.Format = RenderTargetType::ColorRgb10; //RGB10

	RenderBufferDesc depth = RenderBufferDesc();
	depth.ShaderReadable = true;
	depth.Attachment = RenderTargetAttachment::Depth;
	depth.Format = RenderTargetType::Depth32; //32 bit depth
	
	// Our main frame buffer needs a color output, and a depth output
	FrameBuffer::Sptr buffer = std::make_shared<FrameBuffer>(gameWindow->getWidth(), gameWindow->getHeight(), 4);
	buffer->AddAttachment(mainColour);
	//buffer->AddAttachment(normal);
	buffer->AddAttachment(depth);
	buffer->Validate();
}

void Vatista::Game::postProcess()
{
	// Bind the last buffer we wrote to as our source for read operations
	buffer->bind(RenderTargetBinding::Read);

	// Copies the image from lastPass into the default back buffer
	FrameBuffer::Blit({ 0, 0, buffer->GetWidth(), buffer->GetHeight() },
		{ 0, 0, gameWindow->getWidth(), gameWindow->getHeight() },
		BufferFlags::All, MagFilter::Nearest);

	// Unbind the last buffer from read operations, so we can write to it again later
	buffer->unBind();

	//// We grab the application singleton to get the size of the screen
	//florp::app::Application* app = florp::app::Application::Get();
	//FrameBuffer::Sptr mainBuffer = CurrentRegistry().ctx<FrameBuffer::Sptr>();
	//glDisable(GL_DEPTH_TEST);
	//
	//// The last output will start as the output from the rendering
	//FrameBuffer::Sptr lastPass = mainBuffer;
	//
	//for (const PostPass& pass : myPasses) {
	//	// We'll bind our post-processing output as the current render target and clear it
	//	pass.Output->Bind(RenderTargetBinding::Draw);
	//	glClear(GL_COLOR_BUFFER_BIT);
	//
	//	// Set the viewport to be the entire size of the passes output
	//	glViewport(0, 0, pass.Output->GetWidth(), pass.Output->GetHeight());
	//
	//	// Use the post processing shader to draw the fullscreen quad
	//	pass.Shader->Use();
	//	lastPass->GetAttachment(RenderTargetAttachment::Color0)->Bind(0);
	//	pass.Shader->SetUniform("xImage", 0);
	//	pass.Shader->SetUniform("xScreenRes", glm::ivec2(app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight()));
	//	pass.Shader->SetUniform("time", florp::app::Timing::GameTime);
	//
	//	////luts
	//	//pass.Shader->SetUniform("cool", 3);
	//	//pass.Shader->SetUniform("warm", 4);
	//	//pass.Shader->SetUniform("custom", 5);
	//
	//	myFullscreenQuad->Draw();
	//
	//	// Unbind the output pass so that we can read from it
	//	pass.Output->UnBind();
	//	// Update the last pass output to be this passes output
	//	lastPass = pass.Output;
	//
		//// Bind the last buffer we wrote to as our source for read operations
		//lastPass->bind(RenderTargetBinding::Read);
		//
		//// Copies the image from lastPass into the default back buffer
		//FrameBuffer::Blit({ 0, 0, lastPass->GetWidth(), lastPass->GetHeight() },
		//	{ 0, 0, app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight() },
		//	BufferFlags::All, florp::graphics::MagFilter::Nearest);
		//
		//// Unbind the last buffer from read operations, so we can write to it again later
		//lastPass->unBind();
	//}
}

