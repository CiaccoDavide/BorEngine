#pragma once

#include <string>
#include <GL/glew.h>

namespace BorEngine
{
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void linkShaders();

		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();

	private:
		int _numAttributes = 0;

		void compileShader(const std::string& filePath, GLuint id); // type& is a reference

		GLuint _programID = 0;

		GLuint _vertexShaderID = 0;
		GLuint _fragmentShaderID = 0;
	};
}