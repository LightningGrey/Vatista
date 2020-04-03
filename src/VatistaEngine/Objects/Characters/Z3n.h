#pragma once
#include "Character.h"
#include "Utilities/Utils.h"

namespace Vatista {
	class Z3n : public Character {
	public:

		SharedPtr(Z3n);
		Z3n(bool ID, std::vector<Mesh::Sptr>& meshes, Material::Sptr mat);
		~Z3n();
		virtual void update(float dt, GLFWwindow* gameWindow, Character::Sptr p2, AudioEngine::Sptr ae) override;

	};
}