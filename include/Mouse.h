#pragma once

#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

void MouseCallback(GLFWwindow* window, double x, double y);

class Mouse
{
public:

	friend void MouseCallback(GLFWwindow* window, double x, double y);

	static glm::vec2 GetMousePos() { return MousePos; }

private:

	static glm::vec2 MousePos;
};