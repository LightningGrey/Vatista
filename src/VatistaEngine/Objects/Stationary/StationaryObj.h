#pragma once
#include "Objects/GameObject.h"
#include "Graphics/Rendering/Mesh.h"
#include "Graphics/Rendering/Material.h"
#include "Graphics/Camera.h"
#include "Utilities/Utils.h"

namespace Vatista {
	class StationaryObj : public GameObject {
	public:
		SharedPtr(StationaryObj);

		StationaryObj();
		~StationaryObj();

		void Draw(const Vatista::Camera::Sptr& camera);
		void depthdraw(Shader::Sptr shader) {};
	};
}