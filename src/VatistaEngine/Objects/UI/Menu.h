#pragma once
#include "Objects/GameObject.h"
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/Material.h"
#include "Graphics/Camera.h"
#include "Utilities/Utils.h"
#include "Objects/UI/Button.h"

namespace Vatista {
	class Menu : public GameObject {
	public:
		SharedPtr(Menu);

		Menu();
		~Menu();

		void Draw(const Vatista::Camera::Sptr& camera);

		std::vector<Button::Sptr> buttons;

	};
}