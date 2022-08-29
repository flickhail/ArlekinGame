#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <format>
#include <iostream>
#include <string>

//
//~~~~~~~~~~~~~~~~~~~~~~[PUBLIC INTERFACE]~~~~~~~~~~~~~~~~~~~
//
//						[CONSTRUCTORS]

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float fov, float aspectRatio, float near, float far)
	: m_position(position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_Fov{ fov }
	, m_AspectRatio{ aspectRatio }
	, m_NearPlane{ near }
	, m_FarPlane{ far }
{
	m_view = Transform(m_position, m_scale, m_rotation);
	m_projection = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}


//						[GETTERS]

const glm::vec3& Camera::Position()		const { return m_position;   }
const glm::vec3& Camera::Rotation()		const { return m_rotation;   }
const glm::vec3& Camera::Scale()		const { return m_scale;      }
const glm::mat4& Camera::View()			const { return m_view;       }
const glm::mat4& Camera::Projection()	const { return m_projection; }
float			 Camera::Fov()			const { return m_Fov;		 }
float			 Camera::Aspect()		const { return m_AspectRatio;}
float			 Camera::NearPlane()	const { return m_NearPlane;  }
float			 Camera::FarPlane()		const { return m_FarPlane;	 }


//						[SETTERS]

void Camera::Position(const glm::vec3& newPosition)
{
	m_position = newPosition;
	m_view = Transform(m_position, m_scale, m_rotation);
}

void Camera::Rotation(const glm::vec3& newRotation)
{
	m_rotation = newRotation;
	m_view = Transform(m_position, m_scale, m_rotation);
}

void Camera::Projection(float newFov, float newAspectRatio, float newNearPlane, float newFarPlane)
{
	m_Fov = newFov;
	m_AspectRatio = newAspectRatio;
	m_NearPlane = newNearPlane;
	m_FarPlane = newFarPlane;
	m_projection = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}


//						[UTILITY]

std::string Camera::ToString() const
{
	return std::format("Position: [{} {} {}]; Rotation: [{} {} {}]; Scale: [{} {} {}];\n",
				    	m_position.x, m_position.y, m_position.z,
						m_rotation.x, m_rotation.y, m_rotation.z,
						m_scale.x, m_scale.y, m_scale.z);
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


