#include "TitleLayer.h"
#include "Game.h"

namespace Vatista {
	TitleLayer::TitleLayer() : gameWindow(nullptr), clearColour(glm::vec4(0, 0, 0, 1)),
		windowName("Vatista Engine")
	{
	}

	TitleLayer::TitleLayer(std::string newName) : gameWindow(nullptr),
		clearColour(glm::vec4(0, 0, 0, 1)), windowName(newName)
	{
	}

	TitleLayer::~TitleLayer()
	{
	}

	void GlfwWindowResizedCallback(GLFWwindow*, int width, int height) {
		glViewport(0, 0, width, height);
		//Vatista::Window* gameWindow = ; 
		//if (gameWindow != nullptr) { 
		//	gameWindow->getWindow()->resize(width, height); 
		//} 
	}


	void TitleLayer::run(){
	
		init();

		static float previousFrame = (float)glfwGetTime();

		//game loop 
		while (!gameWindow->shouldClose()) {
			static float currentFrame = (float)glfwGetTime();
			static float deltaTime = currentFrame - previousFrame;

			update(deltaTime);
			render(deltaTime);

			glfwSwapBuffers(gameWindow->getWindow());
			glfwPollEvents();

			previousFrame = currentFrame;
		}

		close();
	}

	void TitleLayer::init()
	{
		//window and camera 
		gameWindow = new Vatista::Window(1600, 900, "Alpha Strike");

		orthoCamera = std::make_shared<Vatista::Camera>();
		orthoCamera->SetPosition(glm::vec3(0.0f));
		orthoCamera->LookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		orthoCamera->Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

		//audioEngine = std::make_shared<AudioEngine>();
		//audioEngine->Init();
		//audioEngine->LoadBank("./res/Music and Sound/Master", FMOD_STUDIO_LOAD_BANK_NORMAL);
		//audioEngine->LoadEvent("Music", "{fd7618a1-5880-4a64-ae37-e0f01ef54912}");
		//audioEngine->LoadEvent("LightAttack", "{f64fa79a-565d-4493-b4bf-a73a37c31219}");
		//audioEngine->LoadEvent("HeavyAttack", "{621b2e70-27ea-4900-b397-96cb10366574}");
		//audioEngine->LoadEvent("Dash", "{53dbc862-3dec-411a-9fc4-bb15743c2b6b}");
		//audioEngine->PlayEvent("Music");

		load("./res/Objects/menuInit.txt");

		texture = std::make_shared<Texture2D>();
		texture->loadFile("./res/Objects/Menu/Menu_Texture_PlaceHolder.png");

		Shader::Sptr phong = std::make_shared<Shader>();
		phong->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/blinn-phong.fs.glsl");

		TextureSampler::Sptr NearestMipped = std::make_shared<TextureSampler>();
		NearestMipped->minFilter = MinFilter::NearestMipNearest;
		NearestMipped->magFilter = MagFilter::Nearest;
		NearestMipped->createSampler();

		Material::Sptr planeMat = std::make_shared<Material>(phong);
		planeMat->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
		planeMat->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
		planeMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
		planeMat->Set("a_AmbientPower", 0.5f);
		planeMat->Set("a_LightSpecPower", 0.9f);
		planeMat->Set("a_LightShininess", 256.0f);
		planeMat->Set("a_LightAttenuation", 0.04f);
		planeMat->Set("texSample", texture, NearestMipped);

		menu = std::make_shared<Menu>();
		menu->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		menu->setRotY(-90.0f);
		menu->setMesh(meshList[0]);
		menu->setMat(planeMat);
		ObjectList.push_back(menu);

		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);

	}

	void TitleLayer::close()
	{
		glfwTerminate();
		//audioEngine->Shutdown();
	}

	void TitleLayer::update(float dt)
	{
		//audioEngine->Update();

		//if (glfwGetKey(gameWindow->getWindow(), GLFW_KEY_ENTER), GLFW_PRESS) {
		//	if (menu->buttons[0]->isActive()) {
		//		Vatista::Game* game = new Vatista::Game();
		//		game->run();
		//		delete game;
		//	}
		//}



	}

	void TitleLayer::render(float dt)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		draw(dt);
	}

	void TitleLayer::draw(float dt)
	{
		////draw game objects
		//for (auto object : ObjectList) {
		//	object->Draw(orthoCamera);
		//}
	}

	bool TitleLayer::load(std::string filename)
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
}