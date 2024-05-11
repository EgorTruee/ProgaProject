#pragma once

#include <GL/glew.h>
#include <string>

class Shader
{
public:

	Shader(GLenum type);
	Shader(GLenum type, GLuint count, const GLchar** source, const GLint* length);
	Shader(GLenum type, std::string sourcePath);

	void SetSource(GLuint count, const GLchar** source, const GLint* length);
	void SetSource(std::string sourcePath);

	GLuint GetID() const { return ID; }

	~Shader();

private:

	GLuint ID;

};
