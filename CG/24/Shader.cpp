#include "Shader.h"
#include <iostream>
#include <fstream>	
#include <sstream>

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	if (!compileShader(vertName, GL_VERTEX_SHADER, vertexShader) || !compileShader(fragName, GL_FRAGMENT_SHADER, fragShader))
		return false;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	//glDeleteShader(vertexShader);
	//glDeleteShader(fragShader);

	if (!isValidProgram())
		return false;

	return true;
}

void Shader::Unload()
{
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void Shader::setActive()
{
	glUseProgram(shaderProgram);
}

void Shader::setUniform(const char* name, float xPos, float yPos)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	glUniform2f(loc, xPos, yPos);
}

void Shader::setVec3Uniform(const char* name, float x, float y, float z) const
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	glUniform3f(loc, x, y, z);
}

void Shader::setMatrixUniform(const char* name, glm::mat4 mat) const
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, reinterpret_cast<const float*>(&mat[0][0]));
}

bool Shader::compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	std::ifstream shaderFile(fileName);

	if (!shaderFile.is_open()) {
		std::cerr << "Shader not Found: " << fileName.c_str() << '\n';
		return false;
	}

	std::stringstream sstream;
	sstream << shaderFile.rdbuf();

	std::string contents = sstream.str();
	const char* contentsChar = contents.c_str();

	outShader = glCreateShader(shaderType);

	glShaderSource(outShader, 1, &(contentsChar), nullptr);
	glCompileShader(outShader);

	if (!isCompiled(outShader)) {
		std::cerr << "Failde to compile Shader " << fileName.c_str() << '\n';
		return false;
	}

	return true;
}

bool Shader::isCompiled(GLuint shader)
{
	GLint status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		GLchar errorLog[512];
		memset(errorLog, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, errorLog);
		std::cerr << "GLSL Compile Failed: " << errorLog << '\n';
		return false;
	}

	return true;
}

bool Shader::isValidProgram()
{
	GLint status;

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

	if (status != GL_TRUE) {
		GLchar errorLog[512];
		memset(errorLog, 0, 512);
		glGetProgramInfoLog(shaderProgram, 511, nullptr, errorLog);
		std::cerr << "GLSL Link status: " << errorLog << '\n';
		return false;
	}

	return true;
}
