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

	void setUniform1f(const const char* name, float x) const;
	void setUniform2f(const char* name, float xPos, float yPos) const;
	void setUniform3f(const char* name, float x, float y, float z) const;
	void setMatrixUniform(const char* name, glm::mat4 mat) const;

	void checkUniformLocation(const GLuint& loc) const;

	GLuint GetshaderProgram() const { return shaderProgram; }

private:
	bool compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

	bool isCompiled(GLuint shader);

	bool isValidProgram();

	GLuint vertexShader;
	GLuint fragShader;
	GLuint shaderProgram;
};

