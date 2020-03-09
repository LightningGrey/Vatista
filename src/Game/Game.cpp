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

	lightComposite = std::make_shared<Shader>();
	lightComposite->Load("./res/Shaders/post/post.vs.glsl", "./res/Shaders/post/pointLightPost.fs.glsl");
	lightMat = std::make_shared<Material>(lightComposite);
	finalLighting = std::make_shared<Shader>();
	finalLighting->Load("./res/Shaders/post/post.vs.glsl", "./res/Shaders/post/finalLighting.fs.glsl");
	finalLighting->SetUniform("a_Exposure", 1.0f);

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
	C1 = std::make_shared<Character>(true, meshList[3], testMat);
	ObjectList.push_back(C1);
	
	//Player 2 
	C2 = std::make_shared<Character>(false, meshList[3], testMat);
	ObjectList.push_back(C2);
	

	point = std::make_shared<Light>();
	point->setMesh(meshList[4]);
	Material::Sptr pointMat = std::make_shared<Material>(phong2);
	pointMat->Set("texSample", textureVol, NearestMipped);
	point->setMat(pointMat);

	point->setColour(glm::vec3(1.0f, 1.0f, 0.0f));
	point->setAttenuation(0.50f);
	point->setPos(glm::vec3(0.0f, 0.0f, -10.0f));
	point->setScale(glm::vec3(4.f));
	LightList.push_back(point);


	prop = std::make_shared<StationaryObj>();
	prop->setMesh(meshList[2]);
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
	if (toggles[2]){
		if (point->getControl() == false)
			point->transform();
		else {
			point->controller(gameWindow);
		}

		if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS) {
			point->setControl(!(point->getControl()));
		}
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


	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_1) == GLFW_PRESS)
		toggles[1] = !toggles[1];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_2) == GLFW_PRESS)
		toggles[2] = !toggles[2];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_3) == GLFW_PRESS)
		toggles[3] = !toggles[3];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_4) == GLFW_PRESS)
		toggles[4] = !toggles[4];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_5) == GLFW_PRESS)
		toggles[5] = !toggles[5];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_6) == GLFW_PRESS)
		toggles[6] = !toggles[6];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_7) == GLFW_PRESS) {
		toggles[7] = !toggles[7];
		if (meshCounter == 2) {
			meshCounter = 0;
		} else {
			meshCounter += 1;
		}
		point->setMesh(meshList[4 + meshCounter]);
	}
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_8) == GLFW_PRESS)
		toggles[8] = !toggles[8];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_9) == GLFW_PRESS)
		toggles[9] = !toggles[9];
	if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_KP_0) == GLFW_PRESS)
		toggles[0] = !toggles[0];

}

void Vatista::Game::render(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	draw(dt);
	lightPass();
	postProcess();

}

void Vatista::Game::draw(float)
{
	//mainCamera->BackBuffer->bind();
	//draw game objects
	for (auto object : ObjectList) {
		object->Draw(mainCamera);
	}
	if (toggles[2]) {
		for (auto object : LightList) {
			object->Draw(mainCamera);
		}
	}
	//mainCamera->BackBuffer->unBind();

	// If there's a front buffer, then this camera is double-buffered
	if (mainCamera->FrontBuffer != nullptr) {
		// Swap the back and front buffers
		auto temp = mainCamera->BackBuffer;
		mainCamera->BackBuffer = mainCamera->FrontBuffer;
		mainCamera->FrontBuffer = mainCamera->BackBuffer;
	}

	mainCamera->state.Last = mainCamera->state.Current;
	mainCamera->state.Last.Output = mainCamera->FrontBuffer != nullptr ? mainCamera->BackBuffer : nullptr;
	mainCamera->state.Current.Output = mainCamera->FrontBuffer != nullptr ? mainCamera->FrontBuffer : mainCamera->BackBuffer;
	mainCamera->state.Current.View = mainCamera->GetView();
	mainCamera->state.Current.Projection = mainCamera->Projection;
	mainCamera->state.Current.ViewProjection = mainCamera->GetViewProjection();

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
	//if (!toggles[3] && !toggles[4])
		buffer->AddAttachment(mainColour);
	//if (!toggles[3] && !toggles[5])
		buffer->AddAttachment(normal);
	//if (!toggles[4] && !toggles[5])
		buffer->AddAttachment(depth);
	buffer->Validate();

	mainCamera->BackBuffer = buffer;
	mainCamera->FrontBuffer = buffer->Clone();

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

	//// We'll get the back buffer from the frame state
	//FrameBuffer::Sptr mainBuffer = mainCamera->state.BackBuffer;
	//
	//// Unbind the main framebuffer, so that we can read from it
	////mainBuffer->UnBind();
	//glDisable(GL_DEPTH_TEST);
	//
	//// The last output will start as the output from the rendering
	//FrameBuffer::Sptr lastPass = mainBuffer;
	//
	//float m22 = mainCamera->Projection[2][2];
	//float m32 = mainCamera->Projection[3][2];
	//float nearPlane = (2.0f * m32) / (2.0f * m22 - 2.0f);
	//float farPlane = ((m22 - 1.0f) * nearPlane) / (m22 + 1.0);
	//
	//// We'll iterate over all of our render passes
	//for (const PostPass::Sptr& pass : myPasses) {
	//	if (pass->Enabled) {
	//		// We'll bind our post-processing output as the current render target and clear it
	//		pass->Output->Bind(RenderTargetBinding::Draw);
	//		glClear(GL_COLOR_BUFFER_BIT);
	//		// Set the viewport to be the entire size of the passes output
	//		glViewport(0, 0, pass->Output->GetWidth(), pass->Output->GetHeight());
	//
	//		// Use the post processing shader to draw the fullscreen quad
	//		pass->Shader->Use();
	//		lastPass->Bind(0);
	//		pass->Shader->SetUniform("xImage", 0);
	//
	//		// Expose camera state to shaders
	//		pass->Shader->SetUniform("a_View", state.Current.View);
	//		pass->Shader->SetUniform("a_Projection", state.Current.Projection);
	//		pass->Shader->SetUniform("a_ProjectionInv", glm::inverse(state.Current.Projection));
	//		pass->Shader->SetUniform("a_ViewProjection", state.Current.ViewProjection);
	//		pass->Shader->SetUniform("a_ViewProjectionInv", glm::inverse(state.Current.ViewProjection));
	//
	//		pass->Shader->SetUniform("a_PrevView", state.Last.View);
	//		pass->Shader->SetUniform("a_PrevProjection", state.Last.Projection);
	//		pass->Shader->SetUniform("a_PrevProjectionInv", glm::inverse(state.Last.Projection));
	//		pass->Shader->SetUniform("a_PrevViewProjection", state.Last.ViewProjection);
	//		pass->Shader->SetUniform("a_PrevViewProjectionInv", glm::inverse(state.Last.ViewProjection));
	//
	//		pass->Shader->SetUniform("a_NearPlane", nearPlane);
	//		pass->Shader->SetUniform("a_FarPlane", farPlane);
	//
	//		// We'll bind all the inputs as textures in the order they were added (starting at index 1)
	//		for (size_t ix = 0; ix < pass->Inputs.size(); ix++) {
	//			const auto& input = pass->Inputs[ix];
	//			if (input.Pass == nullptr) {
	//				if (input.UsePrevFrame && state.Last.Output != nullptr) {
	//					state.Last.Output->Bind(ix + 1, input.Attachment);
	//				}
	//				else {
	//					mainBuffer->bind(ix + 1, input.Attachment);
	//				}
	//			}
	//			else {
	//				input.Pass->Output->Bind(ix + 1, input.Attachment);
	//			}
	//		}
	//		pass->Shader->SetUniform("xScreenRes", glm::ivec2(pass->Output->GetWidth(), pass->Output->GetHeight()));
	//		fullscreenQuad->Draw();
	//
	//		// Unbind the output pass so that we can read from it
	//		pass->Output->UnBind();
	//
	//		// Update the last pass output to be this passes output
	//		lastPass = pass->Output;
	//	}
	//}
	//
	//// Bind the last buffer we wrote to as our source for read operations
	//lastPass->bind(RenderTargetBinding::Read);
	//// Copies the image from lastPass into the default back buffer
	//FrameBuffer::Blit({ 0, 0, lastPass->GetWidth(), lastPass->GetHeight() },
	//	{ 0, 0, app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight() }, BufferFlags::All, MagFilter::Nearest);
	//
	//// Unbind the last buffer from read operations, so we can write to it again later
	//lastPass->unBind();


}

void Vatista::Game::lightPass()
{
	
	FrameBuffer::Sptr renderBuffer = mainCamera->state.Current.Output;

	//bind accumulation buffer
	accumulationBuffer->bind();
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//additive blending. depth disable
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	FrameBuffer::Sptr mainBuffer = mainCamera->state.Current.Output;

	// We set up all the camera state once, since we use the same shader for compositing all shadow-casting lights
	lightComposite->Bind();
	lightComposite->SetUniform("a_View", mainCamera->state.Current.View);
	glm::mat4 viewInv = glm::inverse(mainCamera->state.Current.View);
	lightComposite->SetUniform("a_CameraPos", glm::vec3(viewInv * glm::vec4(0, 0, 0, 1)));
	lightComposite->SetUniform("a_ViewProjectionInv", glm::inverse(mainCamera->state.Current.ViewProjection));
	lightComposite->SetUniform("a_MatShininess", 1.0f);
	
	mainBuffer->bind(0, RenderTargetAttachment::Color0); // The color buffer
	mainBuffer->bind(1, RenderTargetAttachment::Depth);
	mainBuffer->bind(2, RenderTargetAttachment::Color1); // The normal buffer
	
	for (auto light : LightList) {
		lightComposite->SetUniform("a_LightPos", light->getPos());
		lightComposite->SetUniform("a_LightColor", light->getColour());
		lightComposite->SetUniform("a_LightAttenuation", light->getAttenuation());
		//lightComposite->SetUniform("a_LightRadius", light->getScale());
		
		light->getMesh()->Draw();
	}

	//unbind accumulation from framebuffer
	accumulationBuffer->unBind();
	glDisable(GL_BLEND);

	// Set the main buffer as the output again
	renderBuffer->bind();
	// We'll use an additive shader for now, this should be a multiply with the albedo of the scene
	finalLighting->Bind();
	// We'll combine the GBuffer color and our lighting contributions
	renderBuffer->bind(1, RenderTargetAttachment::Color0);
	//if (toggles[6])
	accumulationBuffer->bind(2);
	// Render the quad
	fullscreenQuad->Draw();
	// Unbind main buffer to perform multisample blitting
	renderBuffer->unBind();
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