#pragma once
#include <GLM/glm.hpp>
#include <unordered_map>
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"
/*
 Represents settings for a shader
*/
namespace Vatista {
	class Material {
	public:
		SharedPtr(Material);

		Material(const Shader::Sptr& shader) { myShader = shader; }
		virtual ~Material() = default;

		const Shader::Sptr& GetShader() const { return myShader; }
		virtual void Apply();

		void Set(const std::string& name, const glm::mat4& value) { myMat4s[name] = value; }
		void Set(const std::string& name, const glm::vec4& value) { myVec4s[name] = value; }
		void Set(const std::string& name, const glm::vec3& value) { myVec3s[name] = value; }
		void Set(const std::string& name, const float& value) { myFloats[name] = value; }
		void Set(const std::string& name, const Texture::Sptr& value) { myTextures[name] = value; }
		Vatista::Shader::Sptr myShader;
		std::unordered_map<std::string, glm::mat4> myMat4s;
		std::unordered_map<std::string, glm::vec4> myVec4s;
		std::unordered_map<std::string, glm::vec3> myVec3s;
		std::unordered_map<std::string, glm::vec2> myVec2s;
		std::unordered_map<std::string, float> myFloats;
		std::unordered_map<std::string, Texture::Sptr> myTextures;
	};
}