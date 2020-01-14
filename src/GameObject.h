#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "ObjLoader.h"
#include "Camera.h"

class GameObject {
public: 
	GameObject() : Scale(1.0f), model(nullptr), material(nullptr), Position(0.0f), EulerRotDeg(0.0f), Collider(0.0f){} 
	~GameObject() {};

	void Draw(const Vatista::Camera::Sptr& camera);

	//getters and setters
	void setMesh(Vatista::Mesh::Sptr& newMesh) { model = newMesh; }
	void setMat(Vatista::Material::Sptr& newMat) { material = newMat; }
	void setPos(glm::vec3 newPos) { Position = newPos; }
	void setScale(glm::vec3 newScale) { Scale = newScale; }

	void setRot(glm::vec3 newRot) { EulerRotDeg = newRot; }
	//individual rotation components
	void setRotY(float newRot) { EulerRotDeg.y = newRot; }
	
	void setCollider(glm::vec2 newColl) { Collider = newColl; }
	//individual collider components
	void setCollX(float newColl) { Collider.x = newColl; }

	//may not need these all later, check
	Vatista::Mesh::Sptr getMesh() { return model; }
	Vatista::Material::Sptr getMat() { return material; }
	glm::vec3 getPos() { return Position; }
	glm::vec3 getScale() { return Scale; }
	glm::vec3 getRot() { return EulerRotDeg; }
	glm::vec2 getCollider() { return Collider; }

protected:
	//name
	std::string name;

	//model filename data
	std::string modelName;
	std::string textureName;
	const char* shaderName;
	const char* shaderName2;

	//model data
	Vatista::ObjLoader loader;
	Vatista::Mesh::Sptr model;
	Vatista::Texture::Sptr texture;
	Vatista::Shader::Sptr shader;
	Vatista::Material::Sptr material;

	//transforms
	glm::mat4 transform = glm::mat4(0.f);
	glm::vec3 pos;

	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 EulerRotDeg;
	glm::vec2 Collider;

	//only for morphers
	float dt = 0.0167f;
	float morph = 0.0f;
	
	float currentTime = 0.0f;
	float endTime = 10.0f;
};