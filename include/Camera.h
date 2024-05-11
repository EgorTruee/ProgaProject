#pragma once

#include <glm/glm.hpp>

class Camera
{
public:

	Camera(const glm::vec3& position = glm::vec3(0), const glm::vec3& rotation = glm::vec3(0));

	glm::mat4 GetViewTransform() const;
	glm::vec3 GetPosition() const { return Position; }
	glm::vec3 GetRotaion() const { return Rotation; }
	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const { return glm::vec3(0.f, 1.f, 0.f); }
	glm::vec3 GetRight() const { return -glm::normalize(glm::cross(GetUp(), GetForward())); }

	void Move(const glm::vec3& movement) { Position += movement; }
	void Rotate(const glm::vec3& rotation);
	void Update(float delta);

	void SetPosition(const glm::vec3& position) { Position = position; }
	void SetRotation(const glm::vec3& rotation);

private:

	glm::vec3 Position;
	glm::vec3 Rotation;
	float Velocity = 1.f;
};

