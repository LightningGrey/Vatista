#include "menuLayer.h"
#include "MenuLayer.h"
#include "Game.h"

namespace Vatista {
	MenuLayer::MenuLayer() : gameWindow2(nullptr), clearColour2(glm::vec4(0, 0, 0, 1)),
		windowName2("Vatista Engine")
	{
	}

	MenuLayer::MenuLayer(std::string newName) : gameWindow2(nullptr),
		clearColour2(glm::vec4(0, 0, 0, 1)), windowName2(newName)
	{
	}

	MenuLayer::~MenuLayer()
	{
	}


	//runing layer for titlelayer
	void MenuLayer::run() {

		init();

		static float previousFrame = (float)glfwGetTime();

		//game loop 
		while (!gameWindow2->shouldClose()) {
			static float currentFrame = (float)glfwGetTime();
			static float deltaTime = currentFrame - previousFrame;

			update(deltaTime);
			render(deltaTime);

			glfwSwapBuffers(gameWindow2->getWindow());
			glfwPollEvents();

			previousFrame = currentFrame;
		}

		close();
	}



	void MenuLayer::init()
	{
		//window and camera 
		gameWindow2 = new Vatista::Window(1600, 900, "Zeal");

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
		//load stage objects

		load("./res/Objects/menuInit.txt");

		//background png
		texture = std::make_shared<Texture2D>();
		texture->loadFile("./res/Objects/titlescreen/Title_Texture_PlaceHolder.png");

		//title png
		texture2 = std::make_shared<Texture2D>();
		texture2->loadFile("./res/Objects/titlescreen/zeal-_2341.png");

		Shader::Sptr phong = std::make_shared<Shader>();
		phong->Load("./res/Shaders/lighting.vs.glsl", "./res/Shaders/blinn-phong.fs.glsl");

		//texture sampler
		TextureSampler::Sptr NearestMipped = std::make_shared<TextureSampler>();
		NearestMipped->minFilter = MinFilter::NearestMipNearest;
		NearestMipped->magFilter = MagFilter::Nearest;
		NearestMipped->createSampler();

		//title background
		Material::Sptr titlematerial = std::make_shared<Material>(phong);
		titlematerial->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
		titlematerial->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
		titlematerial->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
		titlematerial->Set("a_AmbientPower", 0.5f);
		titlematerial->Set("a_LightSpecPower", 0.9f);
		titlematerial->Set("a_LightShininess", 256.0f);
		titlematerial->Set("a_LightAttenuation", 0.04f);
		titlematerial->Set("texSample", texture2, NearestMipped);

		TitleMenu = std::make_shared<Menu>();
		TitleMenu->setPos(glm::vec3(0.0f, 2.0f, 0.0f));
		TitleMenu->setRotY(-90.0f);
		TitleMenu->setMesh(meshListMenu[0]);
		TitleMenu->setScale(3.0f);
		TitleMenu->setMat(titlematerial);
		ObjectList.push_back(TitleMenu);


		//title name
		Material::Sptr titlematerial2 = std::make_shared<Material>(phong);
		titlematerial2->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
		titlematerial2->Set("a_LightColor", { 1.0f, 1.0f, 1.0f });
		titlematerial2->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
		titlematerial2->Set("a_AmbientPower", 0.5f);
		titlematerial2->Set("a_LightSpecPower", 0.9f);
		titlematerial2->Set("a_LightShininess", 256.0f);
		titlematerial2->Set("a_LightAttenuation", 0.04f);
		titlematerial2->Set("texSample", texture, NearestMipped);

		TitleMenu2 = std::make_shared<Menu>();
		TitleMenu2->setPos(glm::vec3(0.0f, 5.0f, 0.5f));
		TitleMenu2->setRotY(-90.0f);
		TitleMenu2->setMesh(meshListMenu[1]);
		TitleMenu2->setScale(0.5f);
		TitleMenu2->setMat(titlematerial2);
		ObjectList.push_back(TitleMenu2);


		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);

	}

	void MenuLayer::close()
	{
		glfwTerminate();
		//audioEngine->Shutdown();
	}

	void MenuLayer::update(float dt)
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

	void MenuLayer::render(float dt)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		draw(dt);
	}

	void MenuLayer::draw(float dt)
	{
		//draw game objects
		for (auto object : ObjectList) {
			object->Draw(orthoCamera);
		}

	}

	bool MenuLayer::load(std::string filename)
	{

		std::vector<std::string> dataList;
		bool fRead = FileReader::readLines(filename, dataList);

		if (fRead) {
			for (int i = 0; i < dataList.size(); i++) {
				bool vsf = FileReader::vsfRead(dataList[i], meshTitle2);
				if (!vsf) {
					VATISTA_LOG_ERROR("VSF read failed.");
					throw new std::runtime_error("File open failed");
					return false;
				}
				else {
					meshListMenu.push_back(meshTitle2);
				}
			}
		}
		else {
			VATISTA_LOG_ERROR("Init read failed.");
			throw new std::runtime_error("File open failed");
			return false;
		}

		return true;
	}
}
