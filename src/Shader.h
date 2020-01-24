#pragma once

#include <glad/glad.h>
#include <GLM/glm.hpp>
#include <memory>
#include <string>
#include "Utils.h"

namespace Vatista {
	class Shader {
	public:
		SharedPtr(Shader);
		Shader();
		~Shader();

		void Compile(const char* vs_source, const char* fs_source);

		// Loads a shader program from 2 files. vsFile is the path to the vertex shader, and fsFile is
		// the path to the fragment shader
		void Load(const char* vsFile, const char* fsFile);

		void SetUniform(const char* name, const glm::mat4& value);
		void SetUniform(const char* name, const glm::vec4& value);
		void SetUniform(const char* name, const glm::mat3& value);
		void SetUniform(const char* name, const glm::vec3& value);
		void SetUniform(const char* name, const float& value);
		void SetUniform(const char* name, const int& value);

		void Bind();

	private:
		GLuint __CompileShaderPart(const char* source, GLenum type);

		GLuint myShaderHandle;
	};
}
