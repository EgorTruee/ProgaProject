#include "Keyboard.h"

std::array<bool, 1024> Keyboard::Keys = std::array<bool, 1024>();

void KeyboardCollback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		Keyboard::Keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		Keyboard::Keys[key] = false;
	}
}
