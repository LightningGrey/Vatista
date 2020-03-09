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
	gameWindow = new Vatista::Window(1600, 900, "Alpha Strike");

	mainCamera = std::make_shared<Vatista::Camera>();
	mainCamera->SetPosition(glm::vec3(13.0f, 10.0f, 15.0f));
	mainCamera->LookAt(glm::vec3(-20.0f, 2.0f, -40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mainCamera->Projection = glm::perspective(glm::radians(60.0f), 16.f / 9.f, 1.0f, 500.0f);

	load("./res/Objects/init.txt");

	texture = std::make_shared<Texture2D>();
	texture->loadFile("./res/Objects/color-grid.png");
	
	Texture2D::Sptr textureVol = std::make_shared<Texture2D>();
	textureVol->loadFile("./res/Objects/Extra Textures/white.png");
	//Texture2D::Sptr terminalTexture = std::make_shared<Texture2D>();
	//terminalTexture->loadFile("./res/Objects/Terminal/Terminal_Texture.png");

	Texture2D::Sptr textureProp = std::make_shared<Texture2D>();
	textureProp->loadFile("./res/Objects/CrateMedium/CrateMedium_Texture.png");

	Shader::Sptr phong = std::make_shared<Shader>();
	phong->Load("./res/Shaders/passthroughMorph.vs.glsl", "./res/Shaders/blinn-phong.fs.glsl");

	Shader::Sptr phong2 = std::make_shared<Shader>();
	phong2->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/lightingDeferred.fs.glsl");

	//sample testing
	TextureSampler::Sptr NearestMipped = std::make_shared<TextureSampler>();
	NearestMipped->minFilter = MinFilter::NearestMipNearest;
	NearestMipped->magFilter = MagFilter::Nearest;
	NearestMipped->createSampler();

	Material::Sptr testMat = std::make_shared<Material>(phong);
	testMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	testMat->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
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


	textures.emplace_back();
	textures[0] = std::make_shared<Texture2D>();
	textures[0]->loadFile("./res/Objects/Terminal/Background_Texture.png");

	textures.emplace_back();
	textures[1] = std::make_shared<Texture2D>();
	textures[1]->loadFile("./res/Objects/Terminal/Terminal_Texture.png");



	for (int i = 0; i < 2; i++) {
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
	

	for (int i = 0; i < 2; i++) {
		stage = std::make_shared<StationaryObj>();
		stage->setPos(glm::vec3(0.0f, 0.0f, 5.0f));
		stage->setRotY(-90.0f);
		stage->setMesh(meshList[i]);
		stage->setMat(mats[i]);
		ObjectList.push_back(stage);
	}


	//Player 1
	C1 = std::make_shared<Character>(true, meshList[4], testMat);
	ObjectList.push_back(C1);
	
	//Player 2 
	C2 = std::make_shared<Character>(false, meshList[4], testMat);
	ObjectList.push_back(C2);
	
	point = std::make_shared<Light>();
	point->setMesh(meshList[2]);
	Material::Sptr pointMat = std::make_shared<Material>(phong2);
	pointMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	pointMat->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
	pointMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	pointMat->Set("a_AmbientPower", 0.5f);
	pointMat->Set("a_LightSpecPower", 0.9f);
	pointMat->Set("a_LightShininess", 256.0f);
	pointMat->Set("a_LightAttenuation", 0.04f);
	pointMat->Set("texSample", textureVol, NearestMipped);
	//point->setRotY(-90.0f);

	point->setMat(pointMat);
	point->setPos(glm::vec3(0.0f, 0.0f, -10.0f));
	point->setScale(glm::vec3(4.f));
	LightList.push_back(point);


	prop = std::make_shared<StationaryObj>();
	prop->setMesh(meshList[3]);
	Material::Sptr propMat = std::make_shared<Material>(phong2);
	propMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	propMat->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
	propMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	propMat->Set("a_AmbientPower", 0.5f);
	propMat->Set("a_LightSpecPower", 0.9f);
	propMat->Set("a_LightShininess", 256.0f);
	propMat->Set("a_LightAttenuation", 0.04f);
	propMat->Set("texSample", textureProp, NearestMipped);
	prop->setMat(propMat);
	prop->setPos(glm::vec3(0.f, 0.f, 0.f));
	ObjectList.push_back(prop);
	


	bufferCreation();

	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	GAME_LOG_INFO("Init complete.");

}


void Vatista::Game::close()
{
	glfwTerminate();
}

void Vatista::Game::update(float dt)
{
	if (point->getControl() == false)
		point->transform(); 
	else {
		point->controller(gameWindow);
	}

	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS) {
		point->setControl(!(point->getControl()));
	}
	if (C1->getPosX() <= -6.f || C1->collisionCheck(C1->getPos(),C1->getCollider(),C2->getPos(),C2->getCollider()))
		move *= -1.0f;
	C1->setPosX(C1->getPosX() + move);

	float dist = fabs(C1->getPosX() - C2->getPosX());
	if (dist > 15.0f)
		mainCamera->SetPosition(glm::vec3((C1->getPosX() + C2->getPosX()) / 2.0f, 10.0f, 2.5f + (dist / 2.0f)));
	else
		mainCamera->SetPosition(glm::vec3((C1->getPosX() + C2->getPosX()) / 2.0f, 10.0f, 10.0f));
	mainCamera->LookAt(glm::vec3(15.0f, 2.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_1) == GLFW_PRESS)
		toggles[1] != toggles[1];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_2) == GLFW_PRESS)
		toggles[2] != toggles[2];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_3) == GLFW_PRESS)
		toggles[3] != toggles[3];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_4) == GLFW_PRESS)
		toggles[4] != toggles[4];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_5) == GLFW_PRESS)
		toggles[5] != toggles[5];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_6) == GLFW_PRESS)
		toggles[6] != toggles[6];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_7) == GLFW_PRESS)
		toggles[7] != toggles[7];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_8) == GLFW_PRESS)
		toggles[8] != toggles[8];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_9) == GLFW_PRESS)
		toggles[9] != toggles[9];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_0) == GLFW_PRESS)
		toggles[0] != toggles[0];

}

void Vatista::Game::render(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	draw(dt);
	//postProcess();

}

void Vatista::Game::draw(float)
{
	//mainCamera->state.BackBuffer->bind();
	//draw game objects
	for (auto object : ObjectList) {
		object->Draw(mainCamera);
	}
	for (auto object : LightList) {
		object->Draw(mainCamera);
	}
	//mainCamera->state.BackBuffer->unBind();

	//// If there's a front buffer, then this camera is double-buffered
	//if (mainCamera->state.FrontBuffer != nullptr) {
	//	// Swap the back and front buffers
	//	auto temp = mainCamera->state.BackBuffer;
	//	mainCamera->state.BackBuffer = mainCamera->state.FrontBuffer;
	//	mainCamera->state.FrontBuffer = mainCamera->state.BackBuffer;
	//}

}

void Vatista::Game::bufferCreation()
{
	RenderBufferDesc mainColour = RenderBufferDesc();
	mainColour.ShaderReadable = true;
	mainColour.Attachment = RenderTargetAttachment::Color0;
	mainColour.Format = RenderTargetType::ColorRgb8; //RGB8

	RenderBufferDesc normal = RenderBufferDesc();
	normal.ShaderReadable = true;
	normal.Attachment = RenderTargetAttachment::Color1;
	normal.Format = RenderTargetType::ColorRgb10; //RGB10

	RenderBufferDesc depth = RenderBufferDesc();
	depth.ShaderReadable = true;
	depth.Attachment = RenderTargetAttachment::Depth;
	depth.Format = RenderTargetType::Depth32; //32 bit depth

	FrameBuffer::Sptr buffer = std::make_shared<FrameBuffer>(gameWindow->getWidth(), gameWindow->getHeight(), 4);
	buffer->AddAttachment(mainColour);
	buffer->AddAttachment(normal);
	buffer->AddAttachment(depth);
	buffer->Validate();

	mainCamera->state.BackBuffer = buffer;
	mainCamera->state.FrontBuffer = buffer->Clone();

}

void Vatista::Game::postProcess()
{
	//lightPass();


}

void Vatista::Game::lightPass()
{
	//FrameBuffer::Sptr mainBuffer = mainCamera->state.BackBuffer;
	//mainBuffer->bind();
	//
	//float vert[] = {
	//	-1.0f, -1.0f,	0.0f, 0.0f,
	//	1.0f, -1.0f,	1.0f, 0.0f,
	//	-1.0f, 1.0f,	0.0f, 1.0f,
	//	1.0f,  1.0f,	1.0f, 1.0f,	
	//};
	//uint32_t indices[] = {
	//	0, 1, 2,
	//	1, 3, 2
	//};
	//Mesh::Sptr myFullscreenQuad = std::make_shared<Mesh>(vert, 4, indices, 6);
	//
	//
	//Shader::Sptr pointLight = std::make_shared<Shader>();
	//pointLight;
	//
	//
	//RenderBufferDesc mainColor = RenderBufferDesc();
	//mainColor.ShaderReadable = true;
	//mainColor.Attachment = RenderTargetAttachment::Color0;
	//mainColor.Format = RenderTargetType::ColorRgb16F;
	//
	//// We'll use one buffer to accumulate all the lighting
	//FrameBuffer::Sptr accumulationBuffer = std::make_shared<FrameBuffer>(gameWindow->getWidth(), 
	//	gameWindow->getHeight());
	//accumulationBuffer->AddAttachment(mainColor);
	//accumulationBuffer->Validate();

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

