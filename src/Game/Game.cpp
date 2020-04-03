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
	audioEngine->LoadEvent("Stage1 BGNoise", "{57fec8ec-22cd-4b76-be25-95fa3423193d}");
	audioEngine->LoadEvent("Stage2 BGNoise", "{2594ba35-7209-4c59-b5a1-db34faf5e7ed}");
	audioEngine->LoadEvent("Z3n Select", "{90e982a9-8b74-4e2e-8716-fc64701f4297}");
	audioEngine->LoadEvent("Z3n Light Attack", "{f64fa79a-565d-4493-b4bf-a73a37c31219}");
	audioEngine->LoadEvent("Z3n Heavy Attack", "{621b2e70-27ea-4900-b397-96cb10366574}");
	audioEngine->LoadEvent("Z3n Hurt Grunts", "{269721a1-93ec-4eee-a16e-e848b19e15d2}");
	audioEngine->LoadEvent("Z3n Stamina Pants", "{9be48229-ebad-4655-89d0-058437d4e68f}");
	audioEngine->LoadEvent("Z3n Victory", "{6768dd0d-9f95-4d4d-a0b8-dd128cac64f3}");
	audioEngine->LoadEvent("Tashia Select", "{41668ae5-b336-4c4b-89ff-723f79cefa0b}");
	audioEngine->LoadEvent("Tashia Light Attack", "{00effbec-24ed-4d48-a446-61679fd3131e}");
	audioEngine->LoadEvent("Tashia Heavy Attack", "{b0312316-aa68-4993-b48d-1e17beed8374}");
	audioEngine->LoadEvent("Tashia Hurt Grunts", "{ed38b13f-3bfc-44d1-b52d-665580308706}");
	audioEngine->LoadEvent("Tashia Stamina Pants", "{cec94898-5830-4502-90a0-5024932b4d2b}");
	audioEngine->LoadEvent("Tashia Victory", "{d966a2fd-e28e-4fd1-82ac-89e7a9bfe18e}");
	audioEngine->LoadEvent("Block", "{81b5ce8f-0f80-4494-8aa7-dc898cbc38bb}");
	audioEngine->LoadEvent("Dash", "{53dbc862-3dec-411a-9fc4-bb15743c2b6b}");
	audioEngine->PlayEvent("Music");
	audioEngine->PlayEvent("Stage1 BGNoise");
	audioEngine->PlayEvent("Z3n Stamina Pants");
	audioEngine->PlayEvent("Tashia Stamina Pants");
	
	//load stage objects
	bool oLoad = load("./res/Objects/StageOne.txt", stageMeshList, false);
	oLoad = load("./res/Objects/UI.txt", uiMeshList, false);
	oLoad = load("./res/Objects/Tashia.txt", tashiaMeshList, true);
	oLoad = load("./res/Objects/Z3n.txt", z3nMeshList, true);
	
	tashiaTexture = std::make_shared<Texture2D>();
	tashiaTexture->loadFile("./res/Objects/Tashia/Tashia_Texture.png");

	tashiaMap = std::make_shared<Texture2D>();
	tashiaMap->loadFile("./res/Objects/Tashia/Tashia_mesh_map.png");

	z3nTexture = std::make_shared<Texture2D>();
	z3nTexture->loadFile("./res/Objects/Z3n/Z3n_Texture.png");

	z3nMap = std::make_shared<Texture2D>();
	z3nMap->loadFile("./res/Objects/Z3n/Z3n_mesh_map.png");

	propTexture = std::make_shared<Texture2D>();
	propTexture->loadFile("./res/Objects/color-grid.png");

	blankwhite = std::make_shared<Texture2D>();
	blankwhite->loadFile("./res/Objects/White.png");
	blankwhite->bind(99);

	Shader::Sptr character = std::make_shared<Shader>();
	//character->Load("./res/Shaders/passthroughMorph.vs", "./res/Shaders/blinn-phong.fs.glsl");
	character->Load("./res/Shaders/passthroughMorph.vs.glsl", "./res/Shaders/newLightingChar.fs.glsl");

	Shader::Sptr stageProp = std::make_shared<Shader>();
	stageProp->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/newLighting.fs.glsl");
	//stageProp->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/blinn-phong.fs.glsl");

	Shader::Sptr UI = std::make_shared<Shader>();
	UI->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/blinn-phong.fs.glsl");

	Shader::Sptr staminaPhong = std::make_shared<Shader>();//stamina stuff
	staminaPhong->Load("./res/Shaders/StaminaBar.vs.glsl", "./res/Shaders/StaminaBar.fs.glsl");

	Shader::Sptr staminaRamp = std::make_shared<Shader>();//stamina stuff
	staminaRamp->Load("./res/Shaders/StaminaBar.vs.glsl", "./res/Shaders/StaminaBar.fs.glsl");

	//sample testing
	TextureSampler::Sptr NearestMipped = std::make_shared<TextureSampler>();
	NearestMipped->minFilter = MinFilter::NearestMipNearest;
	NearestMipped->magFilter = MagFilter::Nearest;
	NearestMipped->createSampler();

	Material::Sptr tashiaMat = std::make_shared<Material>(character);
	tashiaMat->Set("a_LightPos", { 0.0f, 1.0f, -1.0f });
	tashiaMat->Set("a_LightColor", { 0.0f,0.0f,50.0f });
	tashiaMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	tashiaMat->Set("a_AmbientPower", 0.7f);
	tashiaMat->Set("a_LightSpecPower", 0.9f);
	tashiaMat->Set("a_LightShininess", 256.0f);
	tashiaMat->Set("a_LightAttenuation", 0.04f);
	tashiaMat->Set("texSample", tashiaTexture, NearestMipped);
	Material::Sptr z3nMat = std::make_shared<Material>(character);
	z3nMat->Set("a_LightPos", { 0.0f, 1.0f, -1.0f });
	z3nMat->Set("a_LightColor", { 50.0f,0.0f,0.0f });
	z3nMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	z3nMat->Set("a_AmbientPower", 0.7f);
	z3nMat->Set("a_LightSpecPower", 0.9f);
	z3nMat->Set("a_LightShininess", 256.0f);
	z3nMat->Set("a_LightAttenuation", 0.04f);
	z3nMat->Set("texSample", z3nTexture, NearestMipped);
	
	Material::Sptr stageMat = std::make_shared<Material>(stageProp);
	stageMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	stageMat->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
	stageMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	stageMat->Set("a_AmbientPower", 0.5f);
	stageMat->Set("a_LightSpecPower", 0.9f);
	stageMat->Set("a_LightShininess", 256.0f);
	stageMat->Set("a_LightAttenuation", 0.04f);
	stageMat->Set("texSample", propTexture, NearestMipped);


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
		mats->Set("a_LightAttenuation", 0.04f);
	
		mats->Set("texSample", textures, NearestMipped);
		stage = std::make_shared<StationaryObj>();
		stage->setPos(glm::vec3(0.0f, 0.0f, 5.0f));
		stage->setRotY(-90.0f);
		stage->setMesh(stageMeshList[i]);
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
		stamUIMats[i] = std::make_shared<Material>(UI);
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
	P1 = std::make_shared<Tashia>(true, tashiaMeshList, tashiaMat);
	P1->setNormalMap(tashiaMap);
	ObjectList.push_back(P1);

	//Player 2 
	P2 = std::make_shared<Z3n>(false, z3nMeshList, z3nMat);
	P2->setNormalMap(z3nMap);
	ObjectList.push_back(P2);

	UI1 = std::make_shared<UIObject>();
	UI1->setPos(glm::vec3(-4.8f, 7.f, 0.f));
	UI1->setMesh(uiMeshList[0]);
	UI1->setMat(stamUIMats[P1->getWins()]);
	UI1->setScale(2.5f);
	UIList.push_back(UI1);

	stamUIText = std::make_shared<Texture2D>();
	stamUIText->loadFile("./res/Objects/Tashia/Tashia_render.png");

	Material::Sptr profileMat = std::make_shared<Material>(UI);
	profileMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	profileMat->Set("a_LightColor", { 0.0f, 1.0f, 0.0f });
	profileMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	profileMat->Set("a_AmbientPower", 0.5f);
	profileMat->Set("a_LightSpecPower", 0.9f);
	profileMat->Set("a_LightShininess", 256.0f);
	profileMat->Set("a_LightAttenuation", 0.04f);
	profileMat->Set("texSample", stamUIText, NearestMipped);

	charProfile1 = std::make_shared<UIObject>();
	charProfile1->setPos(glm::vec3(-4.8f, 7.f, 1.f));
	charProfile1->setMesh(uiMeshList[1]);
	charProfile1->setMat(profileMat);
	charProfile1->setScale(2.5f);
	UIList.push_back(charProfile1);

	S1 = std::make_shared<Stamina>();
	S1->setPos(glm::vec3(-4.8f, 7.f, 1.f));
	S1->setMesh(uiMeshList[2]);//3rd one on init.txt 
	S1->setMat(staminaMat/*staminaMat*/);
	//S1->setTexture(textureStamina/*placeholder*/);//might want to use fbo rended texture to change it in real time 
	S1->setScale(2.5f);
	UIList.push_back(S1);


	UI2 = std::make_shared<UIObject>();
	UI2->setPos(glm::vec3(4.8f, 7.f, 0.f));
	UI2->setMesh(uiMeshList[0]);
	UI2->setMat(stamUIMats[P2->getWins()]);

	UI2->setRotY(180.f);
	UI2->setScale(2.5f);
	UIList.push_back(UI2);

	stamUIText = std::make_shared<Texture2D>();
	stamUIText->loadFile("./res/Objects/Z3n/Z3n_render.png");

	profileMat = std::make_shared<Material>(UI);
	profileMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	profileMat->Set("a_LightColor", { 0.0f, 1.0f, 0.0f });
	profileMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	profileMat->Set("a_AmbientPower", 0.5f);
	profileMat->Set("a_LightSpecPower", 0.9f);
	profileMat->Set("a_LightShininess", 256.0f);
	profileMat->Set("a_LightAttenuation", 0.04f);
	profileMat->Set("texSample", stamUIText, NearestMipped);

	charProfile2 = std::make_shared<UIObject>();
	charProfile2->setPos(glm::vec3(4.8f, 7.f, 1.f));
	charProfile2->setMesh(uiMeshList[1]);
	charProfile2->setMat(profileMat);
	charProfile2->setRotY(180.f);
	charProfile2->setScale(2.5f);
	UIList.push_back(charProfile2);

	S2 = std::make_shared<Stamina>();
	S2->setPos(glm::vec3(4.8f, 7.f, 1.f));
	S2->setMesh(uiMeshList[2]);//3rd one on init.txt 
	S2->setMat(staminaMat/*staminaMat*/);
	S2->setRotY(180.f);
	//S2->setTexture(textureStamina/*placeholder*/);//might want to use fbo rended texture to change it in real time 
	S2->setScale(2.5f);
	UIList.push_back(S2);

	bufferCreation(); 

}


void Vatista::Game::close()
{
	glfwTerminate();
	audioEngine->Shutdown();
}

void Vatista::Game::update(float dt)
{
	if (P1->getWins() < 3 && P2->getWins() < 3) {
		P1->update(dt, gameWindow->getWindow(), P2, audioEngine);
		P2->update(dt, gameWindow->getWindow(), P1, audioEngine);
		if (P1->getWins() == 3)
			P1->victoryLine(audioEngine);
		else if (P2->getWins() == 3)
			P2->victoryLine(audioEngine);
		UI1->setMat(stamUIMats[P1->getWins()]);
		UI2->setMat(stamUIMats[P2->getWins()]);
		S1->setStamina(P1->getStamina());
		S2->setStamina(P2->getStamina());
		float dist = fabs(P1->getPosX() - P2->getPosX());
		//P1->setStamina(P1->getStamina() + 10.0f);
		//P2->setStamina(P2->getStamina() + 10.0f);
		if (dist > 5.0f)
			mainCamera->SetPosition(glm::vec3((P1->getPosX() + P2->getPosX()) / 2.0f, 2.0f, 11.0f + (dist / 2.0f)));
		else
			mainCamera->SetPosition(glm::vec3((P1->getPosX() + P2->getPosX()) / 2.0f, 2.0f, 13.5f));
		mainCamera->LookAt(glm::vec3(0.0f, 2.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if(P1->getWins()==3){
	}
	else if(P2->getWins()==3){
	}
	
	audioEngine->Update();

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_ADD)) {
		exposure += 0.01f;
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_SUBTRACT)) {
		exposure -= 0.01f;
	}

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_MULTIPLY)) {
		passes += 1;
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_DIVIDE)) {
		if (passes > 1)
			passes -= 1;
	}


	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_9)) {
		brightness += 0.1f;
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_8)) {
		brightness -= 0.1f;
	}

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_1)) {
		toggles[0] = !toggles[0];
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_2)) {
		toggles[1] = !toggles[1];
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_3)) {
		toggles[2] = !toggles[2];
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_4)) {
		toggles[3] = !toggles[3];
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_5)) {
		toggles[4] = !toggles[4];
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_6)) {
		toggles[5] = !toggles[5];
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_7)) {
		toggles[6] = !toggles[6];
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_8)) {
		toggles[7] = !toggles[7];
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_DECIMAL)){
		toggles[0] = 1;
		toggles[1] = 1;
		toggles[2] = 1;
	}

}

void Vatista::Game::render(float dt)
{

	buffer->bind();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	draw(dt);
}

void Vatista::Game::draw(float)
{

	//draw game objects
	for (auto object : ObjectList) {
		object->getMat()->GetShader()->SetUniform("shadowMap", 5);
		object->Draw(mainCamera, toggles, brightness);
	}
	
	postProcess();

	//draw UI
	for (auto component : UIList) {
		component->Draw(orthoCamera, toggles, brightness);
	}

}

bool Vatista::Game::load(std::string filename, std::vector<Mesh::Sptr>& meshes, bool isChar)
{
	std::vector<std::string> dataList;
	bool fRead = FileReader::readLines(filename, dataList);

	if (fRead) {
		for (int i = 0; i < dataList.size(); i++) {
			bool vsf = isChar ? FileReader::vsfRead2(dataList[i], mesh) : FileReader::vsfRead(dataList[i], mesh);
			if (!vsf) {
				VATISTA_LOG_ERROR("VSF read failed.");
				throw new std::runtime_error("File open failed");
				return false;
			}
			else {
				meshes.push_back(mesh);
			}
		}
	} else {
		VATISTA_LOG_ERROR("Init read failed.");
		throw new std::runtime_error("File open failed");
		return false;
	}

	return true;
}



void Vatista::Game::bufferCreation()
{  
	buffer = std::make_shared<FrameBuffer>();
	
	buffer->createFloatAttachment(gameWindow->getWidth(), gameWindow->getHeight(), RenderTargetAttachment::Color0);
	buffer->createFloatAttachment(gameWindow->getWidth(), gameWindow->getHeight(), RenderTargetAttachment::Color1);
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	buffer->createRenderBuffer(gameWindow->getWidth(), gameWindow->getHeight(), RenderTargetAttachment::DepthStencil,
		RenderTargetType::DepthStencil);
	
	depthbuffer = std::make_shared<FrameBuffer>();
	depthbuffer->createDepthAttachment(SHADOW_WIDTH, SHADOW_HEIGHT, RenderTargetAttachment::Depth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	buffer->bindDefault();


	pingpongBufferH = std::make_shared<FrameBuffer>();
	pingpongBufferH->createFloatAttachment(gameWindow->getWidth(), gameWindow->getHeight(), RenderTargetAttachment::Color0);
	pingpongBufferV = std::make_shared<FrameBuffer>();
	pingpongBufferV->createFloatAttachment(gameWindow->getWidth(), gameWindow->getHeight(), RenderTargetAttachment::Color0);

	 
	bool load = FileReader::vsfRead("mesh_Quad.vsf", fullscreenQuad);
	
	basePost = std::make_shared<Shader>();
	basePost->Load("./res/Shaders/Post-Processing/post.vs.glsl", "./res/Shaders/Post-Processing/hdr.fs.glsl");
	basePost->Bind();
	
	hdrShader = std::make_shared<Shader>(); 
	hdrShader->Load("./res/Shaders/Post-Processing/post.vs.glsl", "./res/Shaders/Post-Processing/hdr.fs.glsl");
	hdrShader->Bind();
	 
	blurShader = std::make_shared<Shader>();
	blurShader->Load("./res/Shaders/Post-Processing/post.vs.glsl", "./res/Shaders/Post-Processing/gaussianblur.fs.glsl");
	blurShader->Bind();
	
	additiveShader = std::make_shared<Shader>();
	additiveShader->Load("./res/Shaders/Post-Processing/post.vs.glsl", "./res/Shaders/Post-Processing/additive.fs.glsl");
	additiveShader->Bind();

}

void Vatista::Game::preProcess()
{
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = -1.0f, far_plane = 1.0f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	depthShader->Bind();
	depthShader->SetUniform("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	depthbuffer->bind();
	glClear(GL_DEPTH_BUFFER_BIT);
\
	for (auto object : ObjectList) {
		object->depthdraw(depthShader);
	}
	depthbuffer->bindDefault();

	// reset viewport
	glViewport(0, 0, 1600, 900);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	depthbuffer->bindColour(5, 0);
}


void Vatista::Game::postProcess()
{
	//Gaussian blur
	blurShader->Bind();
	for (int i = 0; i < passes; i++) {
		pingpongBufferH->bind();
		if(i == 0){
			buffer->bindColour(3, 0);
		}
		else {
			pingpongBufferV->bindColour(3, 0);
		}
		blurShader->SetUniform("xImage", 3);
		blurShader->SetUniform("horizontal", 1);
		fullscreenQuad->Draw();

		pingpongBufferV->bind();
		pingpongBufferH->bindColour(3, 0);
		blurShader->SetUniform("xImage", 3);
		blurShader->SetUniform("horizontal", 0);
		fullscreenQuad->Draw();
	}

	//final additive 
	buffer->bindDefault();
	glDisable(GL_DEPTH_TEST);

	// clear all relevant buffers
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	additiveShader->Bind();
	buffer->bindColour(2, 0);
	pingpongBufferV->bindColour(3, 0);
	additiveShader->SetUniform("mainImage", 2);
	additiveShader->SetUniform("blurImage", 3);
	additiveShader->SetUniform("screenRes", glm::ivec2(gameWindow->getWidth(), gameWindow->getHeight()));
	additiveShader->SetUniform("exposure", exposure);

	fullscreenQuad->Draw();

}

