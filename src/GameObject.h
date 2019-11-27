#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

class GameObject {
public: 
	Vatista::Mesh_sptr Mesh;
	Vatista::Material::Sptr Material;
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 EulerRotDeg;
	glm::vec2 Collider;

	GameObject() : Scale(1.0f), Mesh(nullptr), Material(nullptr), Position(0.0f), EulerRotDeg(0.0f), Collider(0.0f){} 

	void Draw(const Vatista::Camera::Sptr& camera);
};