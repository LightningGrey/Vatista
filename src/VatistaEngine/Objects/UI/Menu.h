#pragma once
#include "Objects/GameObject.h"
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/Material.h"
#include "Graphics/Camera.h"
#include "Utilities/Utils.h"

namespace Vatista {
	class Menu : public GameObject {
	public:
		SharedPtr(Menu);

		Menu();
		~Menu();
	private:


	};
}