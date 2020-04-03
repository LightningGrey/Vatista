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
	audioEngine->LoadEvent("Block", "{81b5ce8f-0f80-4494-8aa7-dc898cbc38bb}");
	audioEngine->PlayEvent("Music");
	
	//load stage objects
	bool oLoad = load("./res/Objects/StageOne.txt", stageMeshList);
	oLoad = load("./res/Objects/UI.txt", uiMeshList);
	oLoad = load("./res/Objects/Z3n.txt", z3nMeshList);
	
	texture = std::make_shared<Texture2D>();
	texture->loadFile("./res/Objects/Z3n/Z3N_Texture.png");

	texture2 = std::make_shared<Texture2D>();
	texture2->loadFile("./res/Objects/color-grid.png");

	Shader::Sptr character = std::make_shared<Shader>();
	//character->Load("./res/Shaders/passthroughMorph.vs", "./res/Shaders/blinn-phong.fs.glsl");
	character->Load("./res/Shaders/passthroughMorph.vs", "./res/Shaders/newLighting.fs.glsl");

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

	Material::Sptr characterMat = std::make_shared<Material>(character);
	characterMat->Set("a_LightPos", { 0.0f, 1.0f, -1.0f });
	characterMat->Set("a_LightColor", { 50.0f, 0.0f, 0.0f }); 
	//characterMat->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
	characterMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	characterMat->Set("a_AmbientPower", 0.7f);
	characterMat->Set("a_LightSpecPower", 0.9f);
	characterMat->Set("a_LightShininess", 256.0f);
	characterMat->Set("a_LightAttenuation", 0.04f);
	characterMat->Set("texSample", texture, NearestMipped);

	Material::Sptr characterMat2 = std::make_shared<Material>(character);
	characterMat2->Set("a_LightPos", { 0.0f, 1.0f, -1.0f });
	characterMat2->Set("a_LightColor", { 0.0f, 0.0f, 50.0f });
	//characterMat2->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
	characterMat2->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	characterMat2->Set("a_AmbientPower", 0.7f);
	characterMat2->Set("a_LightSpecPower", 0.9f);
	characterMat2->Set("a_LightShininess", 256.0f);
	characterMat2->Set("a_LightAttenuation", 0.04f);
	characterMat2->Set("texSample", texture, NearestMipped);
	
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

	stamUIText = std::make_shared<Texture2D>();
	stamUIText->loadFile("./res/Objects/Z3n/Z3n_render.png");

	Material::Sptr profileMat = std::make_shared<Material>(UI);
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
	P1 = std::make_shared<Z3n>(true, z3nMeshList, characterMat);
	ObjectList.push_back(P1);

	//Player 2 
	P2 = std::make_shared<Z3n>(false, z3nMeshList, characterMat2);
	ObjectList.push_back(P2);

	UI1 = std::make_shared<UIObject>();
	UI1->setPos(glm::vec3(-4.8f, 7.f, 0.f));
	UI1->setMesh(uiMeshList[0]);
	UI1->setMat(stamUIMats[P1->getWins()]);
	UI1->setScale(2.5f);
	UIList.push_back(UI1);

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


	//S1.setScale(glm::vec3(0.5f)); 

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
		object->Draw(mainCamera);
	}
	
	postProcess();

	//draw UI
	for (auto component : UIList) {
		component->Draw(orthoCamera);
	}

}

bool Vatista::Game::load(std::string filename, std::vector<Mesh::Sptr>& meshes)
{
	std::vector<std::string> dataList;
	bool fRead = FileReader::readLines(filename, dataList);

	if (fRead) {
		for (int i = 0; i < dataList.size(); i++) {
			bool vsf = FileReader::vsfRead(dataList[i], mesh);
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
	
	depthbuffer->createFloatAttachment(SHADOW_WIDTH, SHADOW_HEIGHT, RenderTargetAttachment::Depth);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
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

	//vblurShader = std::make_shared<Shader>();
	//vblurShader->Load("./res/Shaders/Post-Processing/post.vs.glsl", "./res/Shaders/Post-Processing/gaussianblur.fs.glsl");
	//vblurShader->Bind();
	//vblurShader->SetUniform("isHorizontal", 0);

}

void Vatista::Game::preProcess()
{
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
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
}


void Vatista::Game::postProcess()
{
	//Gaussian blur
	blurShader->Bind();
	for (int i = 0; i < 2; i++) {
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



	//basePost->Bind();
	//buffer->bindColour(2, 0);
	//basePost->SetUniform("xImage", 2);
	//basePost->SetUniform("screenRes", glm::ivec2(gameWindow->getWidth(), gameWindow->getHeight()));

	//hdrShader->Bind();
	//buffer->bindColour(2, 0);
	//hdrShader->SetUniform("xImage", 2);
	//hdrShader->SetUniform("screenRes", glm::ivec2(gameWindow->getWidth(), gameWindow->getHeight()));
	//hdrShader->SetUniform("exposure", exposure);


	fullscreenQuad->Draw();


}

