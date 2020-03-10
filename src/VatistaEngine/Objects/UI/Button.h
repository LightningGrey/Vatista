#pragma once
#include "Objects/GameObject.h"
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/Material.h"
#include "Graphics/Camera.h"
#include "Utilities/Utils.h"

namespace Vatista {
	class Button : public GameObject {
	public:
		SharedPtr(Button);

		Button() : GameObject(), active(false) {};
		~Button() {};

		bool isActive() { return active; };
		void setActive(bool set) { active = set; };

	private:
		bool active;
	};
}