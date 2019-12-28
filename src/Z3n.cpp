#include "Z3n.h"
#include "GLM/gtc/matrix_transform.hpp";

Vatista::Z3n::Z3n()
{
	name = "Z3n";
	modelName = "./res/yun_idle_pose_1.obj";
	textureName = "./res/yuntexturepaint.png";
	shaderName = "./res/passthroughMorph.vs";
	shaderName2 = "./res/blinn-phong.fs.glsl";
	moveSpeed = 0.5f;

	std::vector<uint32_t> indices;
	std::vector<Vertex> vertData;

	bool meshLoad = loader.load(modelName, indices, vertData);
	if (meshLoad) {
		model = std::make_shared<Vatista::Mesh>(indices, indices.size(),
			vertData, vertData.size());
		modelClips.push_back(model);
	}

	texture = std::make_shared<Texture>();
	texture->loadFile(textureName);

	shader = std::make_shared<Shader>();
	shader->Load(shaderName, shaderName2);

	material = std::make_shared<Material>(shader);
	material->Set("a_LightPos", { 0.0f, 0.0f, 1.0f });
	material->Set("a_LightColor", { 0.0f, 1.0f, 0 });
	material->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	material->Set("a_AmbientPower", 0.5f);
	material->Set("a_LightSpecPower", 0.9f);
	material->Set("a_LightShininess", 256.0f);
	material->Set("a_LightAttenuation", 0.04f);

	transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, 3.14f, glm::vec3(0, 1, 0));
	pos = glm::vec3(-1, 0, 0);
	transform = glm::translate(transform, pos);

	EulerRotDeg.y = 90.0f;
	Collider = glm::vec2(0.74f, 1.78f);

}

Vatista::Z3n::~Z3n()
{
}

void Vatista::Z3n::movement(int keyPress)
{
	if (true) {
		this->state = State::WALKING;
	} else if (true){
		this->state = State::RUNNING;
	} else if (true) {
		this->state = State::DASHING;
	}

	this->state = State::IDLE;
}

void Vatista::Z3n::attack(int keyPress)
{
	//if (keyPress == 0) {
		this->state = State::ATTACKING;
		this->state = State::IDLE;
	//} else if (keyPress == 1) {
	//
	//}
}

void Vatista::Z3n::collision()
{
	this->state = State::HITSTUN;
	this->state = State::DEATH;
}
