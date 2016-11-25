#include "GLSLProgram.h"
#include "BorEngineErrors.h"

#include <fstream>
#include <vector>

namespace BorEngine
{
	GLSLProgram::GLSLProgram()
	{

	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		p_programID = glCreateProgram();

		p_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (p_vertexShaderID == 0)
			fatalError("Vertex shader failed to be created!");

		p_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (p_fragmentShaderID == 0)
			fatalError("Fragment shader failed to be created!");

		compileShader(vertexShaderFilePath, p_vertexShaderID);
		compileShader(fragmentShaderFilePath, p_fragmentShaderID);
	}

	void GLSLProgram::linkShaders()
	{

		//Attach our shaders to our program
		glAttachShader(p_programID, p_vertexShaderID);
		glAttachShader(p_programID, p_fragmentShaderID);

		//Link our program
		glLinkProgram(p_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(p_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(p_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(p_programID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(p_programID);
			//Don't leak shaders either.
			glDeleteShader(p_vertexShaderID);
			glDeleteShader(p_fragmentShaderID);

			fatalError("Shaders failed to link:");
			std::printf("%s\n", &(errorLog[0]));
		}
		//Always detach shaders after a successful link.
		glDetachShader(p_programID, p_vertexShaderID);
		glDetachShader(p_programID, p_fragmentShaderID);
		glDeleteShader(p_vertexShaderID);
		glDeleteShader(p_fragmentShaderID);

	}

	void GLSLProgram::addAttribute(const std::string& attributeName)
	{
		glBindAttribLocation(p_programID, p_numAttributes++, attributeName.c_str());
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(p_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found in shader...");
		}
		return location;
	}

	void GLSLProgram::use()
	{
		glUseProgram(p_programID);
		for (int i = 0; i < p_numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < p_numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLProgram::compileShader(const std::string& filePath, GLuint id)
	{

		std::ifstream vertexFile(filePath);
		if (vertexFile.fail())
		{
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(vertexFile, line))
		{
			fileContents += line + "\n";
		}

		vertexFile.close();

		const char* contentsPtr = fileContents.c_str();
		glShaderSource(id, 1, &contentsPtr, nullptr);

		glCompileShader(id);

		// error checking for the shader code:
		GLint isCompiled = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(id);
			fatalError("Shader " + filePath + " failed to compile:");
			std::printf("%s\n", &(errorLog[0]));
		}
	}
}