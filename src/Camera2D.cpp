#include "Camera2D.h"

#include <string>
#include <format>


//					[CONSTRUCTORS]

Camera2D::Camera2D(const glm::vec3& position, float width, float height, float nearPlane, float farPlane)
	: Camera(position, nearPlane, farPlane)
	, m_Width{ width }
	, m_Height{ height }
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

float Camera2D::Scale(float newScale)
{
	// TODO: m_Scale and validation check
	return 0;
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
	return std::format("{}Width: [{}]; Height: [{}]\n", Camera::ToString(), m_Width, m_Height);
}


//						[PRIVATE]

void Camera2D::UpdateViewMatrix()
{
	m_View = Transform(m_Position, { m_Scale, m_Scale, m_Scale });
}

void Camera2D::UpdateProjectionMatrix()
{
	float aspectRatio{ m_Width / m_Height };
	m_Projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, m_NearPlane, m_FarPlane);
}