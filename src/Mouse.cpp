#include "Mouse.h"

glm::vec2 Mouse::MousePos = glm::vec2(0.f);

void MouseCallback(GLFWwindow* window, double x, double y)
{
	Mouse::MousePos = glm::vec2(x, y);
}
