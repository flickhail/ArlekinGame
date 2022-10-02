#include "Camera2D.hpp"

#include <string>
#include <format>

using namespace GameEngine;


//					[CONSTRUCTORS]

Camera2D::Camera2D(const glm::vec3& position, float aspectRatio, float nearPlane, float farPlane)
	: Camera(position, nearPlane, farPlane)
	, m_AspectRatio{ aspectRatio }
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}


//					[SETTERS]

void Camera2D::Position(const glm::vec3& newPosition)
{
	m_Position = newPosition;

	UpdateViewMatrix();
}

void Camera2D::ClipDistance(float nearPlane, float farPlane)
{
	if (nearPlane <= 0 || farPlane <= 0)
		throw std::runtime_error{ "Camera2D.ClipDistance error: the values of near and far planes are non-positive\n" };

	if (nearPlane >= farPlane)
		throw std::runtime_error{ "Camera2D.ClipDistance error: the near plane value is greater than far plane\n" };

	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;

	UpdateProjectionMatrix();
}

void Camera2D::AspectRatio(float newAspectRatio)
{
	if (newAspectRatio <= 0)
		throw std::runtime_error{ "Camera2D.AspectRatio error: new aspect ration value is non-positive\n" };

	m_AspectRatio = newAspectRatio;

	UpdateProjectionMatrix();
}

glm::vec2 Camera2D::Scale(const glm::vec2& newScale)
{
	glm::vec2 previousValue{ m_Scale.x, m_Scale.y };

	m_Scale.x = newScale.x;
	m_Scale.y = newScale.y;

	UpdateProjectionMatrix();
	return previousValue;
}


//						[UTILITY]

// Moves the camera along the moveVector value
void Camera2D::Move(const glm::vec3& moveVector)
{
	m_Position += moveVector;
	UpdateViewMatrix();
}

std::string Camera2D::ToString() const
{
	return std::format("{}Aspect: [{}]\n", Camera::ToString(), m_AspectRatio);
}


//						[PRIVATE]

void Camera2D::UpdateViewMatrix()
{
	m_View = Transform(m_Position);
}

void Camera2D::UpdateProjectionMatrix()
{
	if(m_AspectRatio >= 1)
		m_Projection = glm::ortho(-m_AspectRatio * m_Scale.x, m_AspectRatio * m_Scale.x, -m_Scale.y, m_Scale.y, m_NearPlane, m_FarPlane);
	else
	{
		m_Projection = glm::ortho(-m_Scale.x, m_Scale.x, -m_Scale.y / m_AspectRatio, m_Scale.y / m_AspectRatio, m_NearPlane, m_FarPlane);
	}
}