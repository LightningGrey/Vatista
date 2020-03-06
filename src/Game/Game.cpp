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
	mainCamera->SetPosition(glm::vec3(0.0f, 2.0f, 15.0f));
	mainCamera->LookAt(glm::vec3(0.0f, 2.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mainCamera->Projection = glm::perspective(glm::radians(60.0f), 16.f / 9.f, 1.0f, 200.0f);

	load("./res/Objects/init.txt");

	texture = std::make_shared<Texture2D>();
	texture->loadFile("./res/Objects/Z3n/Z3N_Texture.png");

	//texture3 = std::make_shared<Texture2D>();
	//texture3->loadFile("./res/default.png");

	//Shader::Sptr phong = std::make_shared<Shader>();
	//phong->Load("./res/Shaders/passthroughMorph.vs", "./res/Shaders/blinn-phong.fs.glsl");

	Shader::Sptr phong2 = std::make_shared<Shader>();
	phong2->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/lightingDeferred.fs.glsl");

	//sample testing
	TextureSampler::Sptr NearestMipped = std::make_shared<TextureSampler>();
	NearestMipped->minFilter = MinFilter::NearestMipNearest;
	NearestMipped->magFilter = MagFilter::Nearest;
	NearestMipped->createSampler();

	//Material::Sptr testMat = std::make_shared<Material>(phong);
	//testMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	//testMat->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
	//testMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	//testMat->Set("a_AmbientPower", 0.5f);
	//testMat->Set("a_LightSpecPower", 0.9f);
	//testMat->Set("a_LightShininess", 256.0f);
	//testMat->Set("a_LightAttenuation", 0.04f);
	//testMat->Set("texSample", texture, NearestMipped);

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


	////Player 1
	//C1 = std::make_shared<Character>(true, meshList[0], testMat);
	//ObjectList.push_back(C1);
	//
	////Player 2 
	//C2 = std::make_shared<Character>(false, meshList[0], testMat);
	//ObjectList.push_back(C2);

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

