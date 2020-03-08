#include "Game.h" 
#include "Graphics/Rendering/MeshRenderer.h"
#include <iostream> 
#include "GLM/detail/func_matrix.inl"

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

	//texture = std::make_shared<Texture2D>();
	//texture->loadFile("./res/Objects/Z3n/Z3N_Texture.png");
	
	Texture2D::Sptr textureVol = std::make_shared<Texture2D>();
	textureVol->loadFile("./res/Objects/Extra Textures/white.png");
	//Texture2D::Sptr terminalTexture = std::make_shared<Texture2D>();
	//terminalTexture->loadFile("./res/Objects/Terminal/Terminal_Texture.png");

	//Shader::Sptr phong = std::make_shared<Shader>();
	//phong->Load("./res/Shaders/passthroughMorph.vs", "./res/Shaders/blinn-phong.fs.glsl");

	Shader::Sptr phong2 = std::make_shared<Shader>();
	phong2->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/lightingDeferred.fs.glsl");

	lightComposite = std::make_shared<Shader>();
	lightComposite->Load("./res/Shaders/post/post.vs.glsl", "./res/Shaders/post/pointLightPost.fs.glsl");
	finalLighting = std::make_shared<Shader>();
	finalLighting->Load("./res/Shaders/post/post.vs.glsl", "./res/Shaders/post/finalLighting.fs.glsl");

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


	////Player 1
	//C1 = std::make_shared<Character>(true, meshList[0], testMat);
	//ObjectList.push_back(C1);
	//
	////Player 2 
	//C2 = std::make_shared<Character>(false, meshList[0], testMat);
	//ObjectList.push_back(C2);

	
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
	//LightList.push_back(point);

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
}

void Vatista::Game::render(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	draw(dt);
	//lightPass();
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


	RenderBufferDesc mainColor = RenderBufferDesc();
	mainColor.ShaderReadable = true;
	mainColor.Attachment = RenderTargetAttachment::Color0;
	mainColor.Format = RenderTargetType::ColorRgb16F;

	// We'll use one buffer to accumulate all the lighting
	accumulationBuffer = std::make_shared<FrameBuffer>(gameWindow->getWidth(),
		gameWindow->getHeight());
	accumulationBuffer->AddAttachment(mainColor);
	accumulationBuffer->Validate();

	float vert[] = {
		-1.0f, -1.0f,	0.0f, 0.0f,
		1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f, 1.0f,	0.0f, 1.0f,
		1.0f,  1.0f,	1.0f, 1.0f
	};
	uint32_t indices[] = {
		0, 1, 2,
		1, 3, 2
	};
	fullscreenQuad = std::make_shared<Mesh>(vert, 4, indices, 6);

}

void Vatista::Game::postProcess()
{



}

void Vatista::Game::lightPass()
{
	
	FrameBuffer::Sptr mainBuffer = mainCamera->state.BackBuffer;

	//bind accumulation buffer
	accumulationBuffer->bind();
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//additive blending. depth disable
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);


	// We set up all the camera state once, since we use the same shader for compositing all shadow-casting lights
	lightComposite->Bind();
	//lightComposite->SetUniform("a_View", mainCamera->GetPosition());
	//glm::mat4 viewInv = glm::inverse(state.Current.View);
	lightComposite->SetUniform("a_CameraPos", mainCamera->GetPosition());
	lightComposite->SetUniform("a_ViewProjectionInv", glm::inverse(mainCamera->GetViewProjection()));
	lightComposite->SetUniform("a_MatShininess", 1.0f);
	
	mainBuffer->bind(0, RenderTargetAttachment::Color0); // The color buffer
	mainBuffer->bind(1, RenderTargetAttachment::Depth);
	mainBuffer->bind(2, RenderTargetAttachment::Color1); // The normal buffer
	
	
	for (auto light : LightList) {
		lightComposite->SetUniform("a_LightPos", light->getPos());
		lightComposite->SetUniform("a_LightColor", light->getColour());
		lightComposite->SetUniform("a_LightAttenuation", light->getAttenuation());
		lightComposite->SetUniform("a_LightRadius", light->getScale());
		light->getMesh()->Draw();
	}

	//unbind accumulation from framebuffer
	accumulationBuffer->unBind();
	glDisable(GL_BLEND);

	// Set the main buffer as the output again
	mainBuffer->bind();
	// We'll use an additive shader for now, this should be a multiply with the albedo of the scene
	finalLighting->Bind();
	// We'll combine the GBuffer color and our lighting contributions
	mainBuffer->bind(1, RenderTargetAttachment::Color0);
	accumulationBuffer->bind(2);
	// Render the quad
	fullscreenQuad->Draw();
	// Unbind main buffer to perform multisample blitting
	mainBuffer->unBind();
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

