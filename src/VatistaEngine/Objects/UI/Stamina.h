#pragma once
#include "Objects/GameObject.h"
#include "Graphics/Object Graphics/Mesh.h"
#include "Graphics/Object Graphics/Material.h"
#include "Graphics/Camera.h"
#include "Utilities/Utils.h"

namespace Vatista {
	class Stamina : public GameObject {
	public:
		SharedPtr(Stamina);

		Stamina();
		~Stamina();

		void Draw(const Vatista::Camera::Sptr& camera);

		//change later
		float getStamina() { return stamina; }
		void setStamina(float stam) { stamina = stam; }

	private:
		//change later
		float stamina = 0.0f;
	};
}