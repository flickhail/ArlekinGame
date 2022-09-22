#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <format>
#include <iostream>
#include <string>

//
//~~~~~~~~~~~~~~~~~~~~~~[PUBLIC INTERFACE]~~~~~~~~~~~~~~~~~~~
//

//						[CONSTRUCTORS]

// Creates the undefined camera object
Camera::Camera() { }


// Creates the 2D camera using Settings2D struct
Camera::Camera(const glm::vec3& position, const Settings2D& settings, float nearPlane, float farPlane)
	: m_Position(position)
	, m_Front(0.0f, 0.0f, -1.0f)
	, m_Right(1.0f, 0.0f, 0.0f)
	, m_WorldUp(0.0f, 1.0f, 0.0f)
	, m_LocalUp(0.0f, 1.0f, 0.0f)
	, m_NearPlane{ nearPlane }
	, m_FarPlane{ farPlane }
	, m_ProjectionMode{ ProjectionMode::Orthographic }
	, m_Width{ settings.Width }
	, m_Height{ settings.Height }
{
	/*m_Orthographic = glm::ortho(
				0.0f, static_cast<float>(settings.Width),
				0.0f, static_cast<float>(settings.Height),
				nearPlane, farPlane
	);*/
	m_Orthographic = glm::mat4(1.0f);
	Transform2D(position);
}


// Creates the 3D camera using Settings3D struct
Camera::Camera(const glm::vec3& position, const Settings3D& settings, float nearPlane, float farPlane)
	: m_Position(position)
	, m_Front(0.0f, 0.0f, -1.0f)
	, m_Right(1.0f, 0.0f, 0.0f)
	, m_WorldUp(0.0f, 1.0f, 0.0f)
	, m_LocalUp(0.0f, 1.0f, 0.0f)
	, m_NearPlane{ nearPlane }
	, m_FarPlane{ farPlane }
	, m_ProjectionMode{ ProjectionMode::Perspective }
	, m_FOV{ glm::radians(settings.FOV) }
	, m_AspectRatio{ settings.AspectRatio }
{
	m_Perspective = glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
	Transform3D(position);
}


//						[GETTERS]

const glm::mat4& Camera::View()			const { return m_View; }
const glm::mat4& Camera::Projection()	const 
{
	switch (m_ProjectionMode)
	{
	case ProjectionMode::Orthographic:
		return m_Orthographic;
	case ProjectionMode::Perspective:
		return m_Perspective;
	default:
		throw std::runtime_error{ "Camera.Projection error: camera's projection mode is undefined" };
	}
}


float			 Camera::FOV()			const { return glm::degrees(m_FOV);	}
float			 Camera::Aspect()		const { return m_AspectRatio;				}
float			 Camera::NearPlane()	const { return m_NearPlane;					}
float			 Camera::FarPlane()		const { return m_FarPlane;					}

const glm::vec3& Camera::Position()		const { return m_Position;	}
const glm::vec3& Camera::Front()		const { return m_Front;		}
const glm::vec3& Camera::Right()		const { return m_Right;		}
const glm::vec3& Camera::Up()			const { return m_WorldUp;	}


//						[SETTERS]

void Camera::Position(const glm::vec3& newPosition)
{
	m_Position = newPosition;

	switch (m_ProjectionMode)
	{
	case ProjectionMode::Orthographic:
		Transform2D(newPosition);
		break;
	case ProjectionMode::Perspective:
		LookAt(newPosition + m_Front);
		break;
	default:
		throw std::runtime_error{ "Camera.Position error: projection mode of the camera is undefined" };
	}
}

void Camera::Orthographic(int newWidth, int newHeight)
{
	if (newWidth <= 0 || newHeight <= 0)
		throw std::runtime_error{ "Camera.Orthographic error: new width and height values are non-positive or equal to zero" };

	m_Width = newWidth;
	m_Height = newHeight;
	m_Orthographic = glm::ortho(0.0f, static_cast<float>(newWidth), 0.0f, static_cast<float>(newHeight), m_NearPlane, m_FarPlane);
}

void Camera::Perspective(const Settings3D& newSettings, float nearPlane, float farPlane)
{
	if (m_ProjectionMode == ProjectionMode::Orthographic)
		throw std::runtime_error{ "Camera.Projection error: camera is orthographic. You cannot change the Projection member" };

	m_FOV = newSettings.FOV;
	m_AspectRatio = newSettings.AspectRatio;
	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;
	m_Perspective = glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
}

// Sets the field of view of the camera in degrees
void Camera::FOV(float newFov)
{
	if (m_ProjectionMode == ProjectionMode::Orthographic)
		throw std::runtime_error{ "Camera.FOV error: camera is orthographic, You cannot change the FOV member" };

	m_FOV = glm::radians(glm::clamp(newFov, 1.0f, 90.0f));
	m_Perspective = glm::perspective(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
}

void Camera::Aspect(float newWidth, float newHeight)
{
	if (m_ProjectionMode == ProjectionMode::Orthographic)
		throw std::runtime_error{ "Camera.Aspect error: camera is orthographic, You cannot change the Aspect member" };

	if (newWidth <= 0 || newHeight <= 0)
		throw std::runtime_error{ "Camera.Aspect error: new width or height values are non-positive or equal to zero" };

	m_AspectRatio = newWidth / newHeight;
}

void Camera::ClipDistance(float newNearPlane, float newFarPlane)
{
	if (newNearPlane <= 0 || newFarPlane <= 0)
		throw std::runtime_error{ "Camera.ClipDistance error: new NearPlane and farPlane values are non-positive or equal to zero" };

	switch (m_ProjectionMode)
	{
		case ProjectionMode::Orthographic:
			m_Orthographic = glm::ortho(0.0f, static_cast<float>(m_Width), 0.0f, static_cast<float>(m_Height), newNearPlane, newFarPlane);
			break;
		case ProjectionMode::Perspective:
			m_Perspective = glm::perspective(m_FOV, m_AspectRatio, newNearPlane, newFarPlane);
			break;
	}
}


//						[UTILITY]

void Camera::LookAt(const glm::vec3& target)
{
	if (m_ProjectionMode == ProjectionMode::Orthographic)
		throw std::runtime_error{ "Camera.LookAt error: camera is orthographic. You cannot calculate the LookAt view matrix" };

	m_Front = glm::normalize(target - m_Position);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_LocalUp = glm::normalize(glm::cross(m_Right, m_Front));
	
	m_View = glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp);
}

// There can be problem with default rotation value
void Camera::Transform2D(const glm::vec3& translation, float zRotation)
{
	m_View = Transform(-translation, glm::vec3{ 1.0f }, glm::vec3{ 0.0f, 0.0f, zRotation });
}

// There can be problem with default rotation value
void Camera::Transform3D(const glm::vec3& translation, const glm::vec3& rotation)
{
	m_View = Transform(-translation, glm::vec3{ 1.0f }, rotation);
}


// Sets the rotation of the camera in 3D space. Parameters are in degrees
void Camera::Rotate(float deltaYaw, float deltaPitch)
{
	if (m_ProjectionMode == ProjectionMode::Orthographic)
		throw std::runtime_error{ "Camera.Rotate error: camera is orthographic. You cannot rotate her in 3D space" };

	m_Yaw += glm::radians(deltaYaw);
	m_Pitch += glm::radians(deltaPitch);
	m_Pitch = glm::clamp(m_Pitch, glm::radians(-89.5f), glm::radians(89.5f));

	m_Front.x = cos(m_Pitch) * sin(m_Yaw);
	m_Front.y = sin(m_Pitch);
	m_Front.z = -cos(m_Pitch) * cos(m_Yaw);

	m_Front = glm::normalize(m_Front);
}


void Camera::Zoom(float scrollOffset)
{
	static float tmp{ m_FOV };

	tmp -= glm::radians(scrollOffset);
	tmp = glm::clamp(tmp, glm::radians(1.0f), m_FOV);
	m_Perspective = glm::perspective(tmp, m_AspectRatio, m_NearPlane, m_FarPlane);
}


std::string Camera::ToString() const
{
	return std::format("Position: [{:.3} {:.3} {:.3}]; Front: [{:.3} {:.3} {:.3}]; Right: [{:.3} {:.3} {:.3}]; LocalUp: [{:.3} {:.3} {:.3}];\n"
							"FOV: [{}]; Width: [{}]; Height: [{}]; Aspect: [{:.3}];\n"
							"Pitch: [{:.3}]; Yaw: [{:.3}]\n"
							"Clip distance, Near: [{}]; Far[{}]\n",
				    	m_Position.x,	m_Position.y,	m_Position.z,
						m_Front.x,		m_Front.y,		m_Front.z,
						m_Right.x,		m_Right.y,		m_Right.z,
						m_LocalUp.x,	m_LocalUp.y,	m_LocalUp.z,
						glm::degrees(m_FOV), m_Width, m_Height, m_AspectRatio,
						glm::degrees(m_Pitch), glm::degrees(m_Yaw),
						m_NearPlane, m_FarPlane);
}


//						[OPERATIONS]

std::ostream& operator<<(std::ostream& out, const Camera& camera)
{
	out << camera.ToString();
	return out;
}



//~~~~~~~~~~~~~~~~~~~~~~[PRIVATE]~~~~~~~~~~~~~~~~~~~~~~~~

// Calculates the view matrix
glm::mat4 Camera::Transform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation) const
{
	glm::mat4 transform = glm::mat4(1.0f);

	transform = glm::translate(transform, translation);
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	return transform;
}

