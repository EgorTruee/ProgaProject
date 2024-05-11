#pragma once

#include <GLFW/glfw3.h>
#include <array>

void KeyboardCollback(GLFWwindow* window, int key, int scancode, int action, int mode);

class Keyboard
{
public:

	friend void KeyboardCollback(GLFWwindow* window, int key, int scancode, int action, int mode);

	static bool GetKeyValue(int key) { return Keys[key]; }

private:

	static std::array<bool, 1024> Keys;
};