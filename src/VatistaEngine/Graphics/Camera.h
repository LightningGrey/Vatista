#pragma once
#include <GLM/glm.hpp>
#include <memory>
#include <GLM/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Utilities/Utils.h"
#include "Post-Processing/FrameBuffer.h"

namespace Vatista {
	struct CamState {
		// True if this camera is the main camera for the scene (ie, what gets rendered fullscreen)
		bool              IsMainCamera;
		// The render target that this camera will render to
		FrameBuffer::Sptr BackBuffer;
		// The previous frame this camera rendered
		FrameBuffer::Sptr FrontBuffer;
		// The Color to clear to when using this camera
		glm::vec4         ClearCol = glm::vec4(0, 0, 0, 1);

		// The projection matrix for this camera
		glm::mat4         Projection;
	};

	class Camera {
	public:
		SharedPtr(Camera);

		Camera();
		virtual ~Camera();

		glm::mat4 Projection;

		CamState state;

		const glm::mat4& GetView() const { return myView; }
		inline glm::mat4 GetViewProjection() const { return Projection * myView; }
		const glm::vec3& GetPosition() const { return myPosition; }
		void SetPosition(const glm::vec3& pos);
		// Gets the front facing vector of this camera
		inline glm::vec3 GetForward() const { return glm::vec3(-BackX, -BackY, -BackZ); }
		// Gets the up vector of this camera
		inline glm::vec3 GetUp() const { return glm::vec3(UpX, UpY, UpZ); }
		// Gets the right hand vector of this camera
		inline glm::vec3 GetRight() const { return glm::vec3(-LeftX, -LeftY, -LeftZ); }
		void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 0, 1));
		void Rotate(const glm::quat& rot);
		void Rotate(const glm::vec3& rot) { Rotate(glm::quat(rot)); }
		void Move(const glm::vec3& local);

	protected:
		glm::vec3 myPosition;

		// We are going to declare a union between multiple data members
		// These will exist in the same spot in memory, but can be accessed
		// with different names
		union {
			// The base type is our view matrix
			mutable glm::mat4 myView;
			// The next types are all grouped together
			struct {
				float
					LeftX, UpX, BackX, M03,
					LeftY, UpY, BackY, M13,
					LeftZ, UpZ, BackZ, M33,
					TransX, TransY, TransZ, M43;
			};
		};

	};
}