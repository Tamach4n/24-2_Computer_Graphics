#pragma once
#include <string>
#include <glew.h>

class Shader
{
public:
	Shader();

	~Shader();

	bool Load(const std::string& vertName, const std::string& fragName);

	void Unload();

	void setActive();

	void setUniform(const char* name, float xPos, float yPos);

	GLuint GetshaderProgram() const { return shaderProgram; }

private:
	bool compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	bool isCompiled(GLuint shader);

	bool isValidProgram();

	GLuint vertexShader;
	GLuint fragShader;
	GLuint shaderProgram;
};

