#include "Camera.h"
#include <format>
#include <iostream>
#include <string>

//						[CONSTRUCTORS]

Camera::Camera(const glm::vec3& pos, float nearPlane, float farPlane) noexcept
	: m_Position{ pos }
	, m_NearPlane{ nearPlane }
	, m_FarPlane{ farPlane }
{ }


//						[SETTERS]

float Camera::Speed(float newSpeed)
{
	if (newSpeed < 0)
		throw std::runtime_error{ "Camera.Speed error: the speed value is negative\n" };

	float prevSpeed = m_Speed;
	m_Speed = newSpeed;

	return prevSpeed;
}


//						[UTILITY]

std::string Camera::ToString() const
{
	return std::format("Position: [{:.3}; {:.3}; {:.3}]; Near: [{:.3}]; Far: [{:.3}]\n", m_Position.x, m_Position.y, m_Position.z, m_NearPlane, m_FarPlane);
}


//						[OPERATIONS]

std::ostream& operator<<(std::ostream& out, const Camera& camera)
{
	out << camera.ToString();
	return out;
}


//						[PRIVATE]

//						[UTILITY]

glm::mat4 Camera::Transform(const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotation) noexcept
{
	glm::mat4 transform{ 1.0f };

	transform = glm::translate(transform, -pos);
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, -rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, -rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, -rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	return transform;
}