#pragma once
#include <string>
#include <gl/glew.h>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader();

	~Shader();

	bool Load(const std::string& vertName, const std::string& fragName);

	void Unload();

	void setActive();

	void setUniform(const char* name, float xPos, float yPos) const;
	void setUniform3(const char* name, float x, float y, float z) const;
	void setUniformM(const char* name, glm::mat4 mat) const;

	GLuint GetshaderProgram() const { return shaderProgram; }

private:
	bool compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	bool isCompiled(GLuint shader);

	bool isValidProgram();

	GLuint vertexShader;
	GLuint fragShader;
	GLuint shaderProgram;
};

