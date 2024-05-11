#include "Shader.h"

#include <stdexcept>
#include <sstream>
#include <fstream>

Shader::Shader(GLenum type) :
	ID(glCreateShader(type))
{
}

Shader::Shader(GLenum type, GLuint count, const GLchar** source, const GLint* length) :
	Shader(type)
{
	SetSource(count, source, length);
}

Shader::Shader(GLenum type, std::string sourcePath) :
	Shader(type)
{
	SetSource(sourcePath);
}

void Shader::SetSource(GLuint count, const GLchar** source, const GLint* length)
{
	glShaderSource(ID, count, source, length);
	glCompileShader(ID);

	GLint Success;

	glGetShaderiv(ID, GL_COMPILE_STATUS, &Success);

	if (!Success)
	{
		GLchar InfoLog[512];
		std::stringstream ss;

		glGetShaderInfoLog(ID, 512, NULL, InfoLog);

		ss << "Shader compilation failed:\n" << InfoLog << std::endl;

		throw std::runtime_error(ss.str());
	}
}

void Shader::SetSource(std::string sourcePath)
{
	std::ifstream fin;
	std::string ShaderSource;
	std::stringstream ss;

	fin.exceptions(std::ifstream::badbit);
	fin.open(sourcePath);

	if (!fin.is_open())
	{
		throw std::runtime_error(std::string("File didn't open ") + sourcePath);
	}

	ss << fin.rdbuf();

	fin.close();

	ShaderSource = ss.str();

	const GLchar* ShaderSourseCStr = ShaderSource.c_str();

	SetSource(1, &ShaderSourseCStr, NULL);
}

Shader::~Shader()
{
	glDeleteShader(ID);
}