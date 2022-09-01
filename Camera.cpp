#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <format>
#include <iostream>
#include <string>

//
//~~~~~~~~~~~~~~~~~~~~~~[PUBLIC INTERFACE]~~~~~~~~~~~~~~~~~~~
//

//						[CONSTRUCTORS]

Camera::Camera(const glm::vec3& position, const glm::vec3& target, float fov, float aspectRatio, float near, float far)
	: m_Position(position)
	, m_Front(glm::normalize(target - m_Position))
	, m_WorldUp(0.0f, 1.0f, 0.0f)
	, m_Right(glm::normalize(glm::cross(m_Front, m_WorldUp)))
	, m_LocalUp(glm::cross(m_Right, m_Front))
	, m_Fov{ fov }
	, m_AspectRatio{ aspectRatio }
	, m_NearPlane{ near }
	, m_FarPlane{ far }
	, m_Pitch{ 0.0f }
	, m_Yaw{ 0.0f }
{
	m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}


//						[GETTERS]

const glm::mat4& Camera::Projection()	const { return m_Projection; }
float			 Camera::Fov()			const { return m_Fov;		 }
float			 Camera::Aspect()		const { return m_AspectRatio;}
float			 Camera::NearPlane()	const { return m_NearPlane;  }
float			 Camera::FarPlane()		const { return m_FarPlane;	 }

const glm::vec3& Camera::Position()		const { return m_Position; }
const glm::vec3& Camera::Front()		const { return m_Front; }
const glm::vec3& Camera::Right()		const { return m_Right; }


//						[SETTERS]

void Camera::Position(const glm::vec3& newPosition)
{
	m_Position = newPosition;
}

void Camera::Projection(float newFov, float newAspectRatio, float newNearPlane, float newFarPlane)
{
	m_Fov = newFov;
	m_AspectRatio = newAspectRatio;
	m_NearPlane = newNearPlane;
	m_FarPlane = newFarPlane;
	m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}

void Camera::Fov(float newFov)
{
	m_Fov = glm::clamp(newFov, glm::radians(1.0f), glm::radians(90.0f));
	m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}


//						[UTILITY]

glm::mat4 Camera::LookAt(const glm::vec3& target)
{
	m_Front = glm::normalize(target - m_Position);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_LocalUp = glm::normalize(glm::cross(m_Right, m_Front));
	
	return glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp);
}

void Camera::Rotate(float deltaYaw, float deltaPitch)
{
	m_Yaw += deltaYaw;
	m_Pitch += deltaPitch;
	m_Pitch = glm::clamp(m_Pitch, -89.5f, 89.5f);

	m_Front.x = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
	m_Front.y = sin(glm::radians(m_Pitch));
	m_Front.z = -cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));

	m_Front = glm::normalize(m_Front);
}

void Camera::Zoom(float scrollOffset)
{
	static float tmp{ m_Fov };

	tmp -= glm::radians(scrollOffset);
	tmp = glm::clamp(tmp, glm::radians(1.0f), m_Fov);
	m_Projection = glm::perspective(tmp, m_AspectRatio, m_NearPlane, m_FarPlane);
}

std::string Camera::ToString() const
{
	return std::format("Position: [{} {} {}]; Front: [{} {} {}]; Right: [{} {} {}]; LocalUp: [{} {} {}];\n",
				    	m_Position.x,	m_Position.y,	m_Position.z,
						m_Front.x,		m_Front.y,		m_Front.z,
						m_Right.x,		m_Right.y,		m_Right.z,
						m_LocalUp.x,	m_LocalUp.y,	m_LocalUp.z);
}


//						[OPERATIONS]

std::ostream& operator<<(std::ostream& out, const Camera& camera)
{
	out << camera.ToString();
	return out;
}



//~~~~~~~~~~~~~~~~~~~~~~[PRIVATE]~~~~~~~~~~~~~~~~~~~~~~~~

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

