#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "Keyboard.h"
#include "Mouse.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation) :
	Position(position),
	Rotation(rotation)
{
}

glm::mat4 Camera::GetViewTransform() const
{
	return glm::lookAt(Position, Position - GetForward(), glm::vec3(0.f, 1.f, 0.f));
}

glm::vec3 Camera::GetForward() const
{
	glm::vec3 Forward = glm::vec3(cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x)),
		sin(glm::radians(Rotation.y)),
		cos(glm::radians(Rotation.y)) * sin(glm::radians(Rotation.x)));

	return glm::normalize(Forward);
}

void Camera::Rotate(const glm::vec3& rotation)
{
	Rotation += rotation;

	if (Rotation.y > 89.f)
	{
		Rotation.y = 89.f;
	}
	else if (Rotation.y < -89.f)
	{
		Rotation.y = -89.f;
	}
}

void Camera::Update(float delta)
{
	const glm::vec2 Center = glm::vec2(500, 500);
	if (Keyboard::GetKeyValue(GLFW_KEY_W))
	{
		Move(Velocity * delta * GetForward());
	}
	if (Keyboard::GetKeyValue(GLFW_KEY_S))
	{
		Move(-Velocity * delta * GetForward());
	}
	if (Keyboard::GetKeyValue(GLFW_KEY_D))
	{
		Move(Velocity * delta * GetRight());
	}
	if (Keyboard::GetKeyValue(GLFW_KEY_A))
	{
		Move(-Velocity * delta * GetRight());
	}
	if (Keyboard::GetKeyValue(GLFW_KEY_SPACE))
	{
		Move(-Velocity * delta * GetUp());
	}
	if (Keyboard::GetKeyValue(GLFW_KEY_C))
	{
		Move(Velocity * delta * GetUp());
	}
	SetRotation(glm::vec3(Mouse::GetMousePos() - Center, 0));
}

void Camera::SetRotation(const glm::vec3& rotation)
{
	Rotation = rotation;

	if (Rotation.y > 89.f)
	{
		Rotation.y = 89.f;
	}
	else if (Rotation.y < -89.f)
	{
		Rotation.y = -89.f;
	}
}
