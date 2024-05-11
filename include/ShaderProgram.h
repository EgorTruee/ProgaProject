#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <glm/ext/matrix_float4x4.hpp>

class Shader;

class ShaderProgram
{
public:

	ShaderProgram();
	ShaderProgram(const Shader& Vertex, const Shader& Fragment);
	ShaderProgram(const std::vector<Shader>& shaders);

	void AttachShader(const Shader& shader);
	void AttachShader(const GLuint& shaderID);
	void DetachShader(const Shader& shader);
	void DetachShader(const GLuint& shaderID);
	void Link();
	void Use();
	void UnUse();
	void Uniform(const std::string& uniform, float value);
	void Uniform(const std::string& uniform, GLint value);
	void Uniform(const std::string& uniform, float v0, float v1, float v2, float v3);
	void Uniform(const std::string& uniform, const glm::mat4& value);

	~ShaderProgram();

private:

	GLuint Program;
	std::map<std::string, GLint> Uniforms;
};

