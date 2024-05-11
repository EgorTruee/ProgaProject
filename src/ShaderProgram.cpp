#include "ShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

ShaderProgram::ShaderProgram() :
	Program(glCreateProgram())
{
}

ShaderProgram::ShaderProgram(const Shader& Vertex, const Shader& Fragment) :
	ShaderProgram()
{
	AttachShader(Vertex);
	AttachShader(Fragment);

	Link();

	DetachShader(Vertex);
	DetachShader(Fragment);
}

ShaderProgram::ShaderProgram(const std::vector<Shader>& shaders) :
	ShaderProgram()
{
	for (auto& i : shaders)
	{
		AttachShader(i);
	}
	Link();
	for (auto& i : shaders)
	{
		DetachShader(i);
	}
}

void ShaderProgram::AttachShader(const Shader& shader)
{
	glAttachShader(Program, shader.GetID());
}

void ShaderProgram::AttachShader(const GLuint& shaderID)
{
	glAttachShader(Program, shaderID);
}

void ShaderProgram::DetachShader(const Shader& shader)
{
	glDetachShader(Program, shader.GetID());
}

void ShaderProgram::DetachShader(const GLuint& shaderID)
{
	glDetachShader(Program, shaderID);
}

void ShaderProgram::Link()
{
	glLinkProgram(Program);

	GLint Success;

	glGetProgramiv(Program, GL_LINK_STATUS, &Success);

	if (!Success)
	{
		GLchar InfoLog[512];
		std::stringstream Stream;

		glGetProgramInfoLog(Program, 512, NULL, InfoLog);

		Stream << "Programm linking failed: " << InfoLog;

		throw std::runtime_error(Stream.str());
	}
}

void ShaderProgram::Use()
{
	glUseProgram(Program);
}

void ShaderProgram::UnUse()
{
	glUseProgram(0);
}

void ShaderProgram::Uniform(const std::string& uniform, float value)
{
	auto I = Uniforms.find(uniform);

	if (I == Uniforms.end())
	{
		I = Uniforms.emplace(uniform, glGetUniformLocation(Program, uniform.c_str())).first;
	}
	glUniform1f(I->second, value);
}

void ShaderProgram::Uniform(const std::string& uniform, GLint value)
{
	GLint CurrentProgram = 0;

	glGetIntegerv(GL_CURRENT_PROGRAM, &CurrentProgram);

	glUseProgram(Program);

	auto I = Uniforms.find(uniform);

	if (I == Uniforms.end())
	{
		I = Uniforms.emplace(uniform, glGetUniformLocation(Program, uniform.c_str())).first;
	}
	glUniform1i(I->second, value);
	glUseProgram(CurrentProgram);
}

void ShaderProgram::Uniform(const std::string& uniform, float v0, float v1, float v2, float v3)
{
	GLint CurrentProgram = 1;

	glGetIntegerv(GL_CURRENT_PROGRAM, &CurrentProgram);

	glUseProgram(Program);

	auto I = Uniforms.find(uniform);

	if (I == Uniforms.end())
	{
		I = Uniforms.emplace(uniform, glGetUniformLocation(Program, uniform.c_str())).first;
	}
	glUniform4f(I->second, v0, v1, v2, v3);
	glUseProgram(CurrentProgram);

}

void ShaderProgram::Uniform(const std::string& uniform, const glm::mat4& value)
{
	GLint CurrentProgram = 0;

	glGetIntegerv(GL_CURRENT_PROGRAM, &CurrentProgram);

	glUseProgram(Program);

	auto I = Uniforms.find(uniform);

	if (I == Uniforms.end())
	{
		I = Uniforms.emplace(uniform, glGetUniformLocation(Program, uniform.c_str())).first;
	}
	glUniformMatrix4fv(I->second, 1, GL_FALSE, glm::value_ptr(value));
	glUseProgram(CurrentProgram);

}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(Program);
}
