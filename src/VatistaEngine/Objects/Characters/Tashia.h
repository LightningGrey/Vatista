#pragma once
#include "Character.h"
#include "Utilities/Utils.h"

namespace Vatista {
	class Tashia : public Character {
	public:

		SharedPtr(Tashia);
		Tashia(bool ID, std::vector<Mesh::Sptr>& meshes, Material::Sptr mat);
		~Tashia();
		virtual void update(float dt, GLFWwindow* gameWindow, Character::Sptr p2, AudioEngine::Sptr ae) override;

	};
}