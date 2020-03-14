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
	mainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
	mainCamera->LookAt(glm::vec3(0.0f, 2.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mainCamera->Projection = glm::perspective(glm::radians(60.0f), 16.f / 9.f, 1.0f, 500.0f);

	//audio
	audioEngine = std::make_shared<AudioEngine>();
	audioEngine->Init();
	audioEngine->LoadBank("./res/Music and Sound/Master", FMOD_STUDIO_LOAD_BANK_NORMAL);
	audioEngine->LoadEvent("CarCrash", "{8fa30912-1d55-4c75-addb-edb8a014c40c}");

	audioEngine->PlayEvent("CarCrash");
	
	load("./res/Objects/init.txt");

	texture = std::make_shared<Texture2D>();
	texture->loadFile("./res/Objects/color-grid.png");

	Shader::Sptr car = std::make_shared<Shader>();
	car->Load("./res/Shaders/passthroughMorph.vs", "./res/Shaders/blinn-phong.fs.glsl");

	//sample testing
	TextureSampler::Sptr NearestMipped = std::make_shared<TextureSampler>();
	NearestMipped->minFilter = MinFilter::NearestMipNearest;
	NearestMipped->magFilter = MagFilter::Nearest;
	NearestMipped->createSampler();

	Material::Sptr carMat = std::make_shared<Material>(car);
	carMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	carMat->Set("a_LightColor", { 0.0f, 0.0f, 0.0f });
	carMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	carMat->Set("a_AmbientPower", 0.5f);
	carMat->Set("a_LightSpecPower", 0.9f);
	carMat->Set("a_LightShininess", 256.0f);
	carMat->Set("a_LightAttenuation", 0.04f);
	carMat->Set("texSample", texture, NearestMipped);

	carSphere = std::make_shared<StationaryObj>();
	carSphere->setMesh(meshList[0]);
	carSphere->setMat(carMat);
	carSphere->setPos(glm::vec3(-10.0f, 0.0f, 0.0f));
	ObjectList.push_back(carSphere);

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
	
	audioEngine->Update();


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
	//RenderBufferDesc mainColour = RenderBufferDesc();
	//mainColour.ShaderReadable = true;
	//mainColour.Attachment = RenderTargetAttachment::Color0;
	//mainColour.Format = RenderTargetType::ColorRgb8; //RGB8
	//
	////RenderBufferDesc normal = RenderBufferDesc();
	////normal.ShaderReadable = true;
	////normal.Attachment = RenderTargetAttachment::Color1;
	////normal.Format = RenderTargetType::ColorRgb10; //RGB10
	//
	//RenderBufferDesc depth = RenderBufferDesc();
	//depth.ShaderReadable = true;
	//depth.Attachment = RenderTargetAttachment::Depth;
	//depth.Format = RenderTargetType::Depth32; //32 bit depth
	//
	//// Our main frame buffer needs a color output, and a depth output
	//FrameBuffer::Sptr buffer = std::make_shared<FrameBuffer>(gameWindow->getWidth(), gameWindow->getHeight(), 4);
	//buffer->AddAttachment(mainColour);
	////buffer->AddAttachment(normal);
	//buffer->AddAttachment(depth);
	//buffer->Validate();
}

void Vatista::Game::postProcess()
{
	//// Bind the last buffer we wrote to as our source for read operations
	//buffer->bind(RenderTargetBinding::Read);
	//
	//// Copies the image from lastPass into the default back buffer
	//FrameBuffer::Blit({ 0, 0, buffer->GetWidth(), buffer->GetHeight() },
	//	{ 0, 0, gameWindow->getWidth(), gameWindow->getHeight() },
	//	BufferFlags::All, MagFilter::Nearest);
	//
	//// Unbind the last buffer from read operations, so we can write to it again later
	//buffer->unBind();

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

