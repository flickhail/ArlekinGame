#pragma once
#include <glm/glm.hpp>
#include <string>
#include <iostream>

class Camera
{
public:
	//						[CONSTRUCTORS]

	Camera(const glm::vec3& position, const glm::vec3& target, float fov, float aspectRatio, float near, float far);
	Camera() = delete;
	Camera(const Camera& camera) = delete;


	//						[GETTERS]

	const glm::vec3& Position()		const;
	const glm::mat4& Projection()	const;
	float			 Fov()			const;
	float			 Aspect()		const;
	float			 NearPlane()	const;
	float			 FarPlane()		const;

	const glm::vec3& Front()		const;
	const glm::vec3& Right()		const;

	//						[SETTERS]

	void Position	(const glm::vec3& newPosition);
	void Projection	(float newFov, float newAspectRatio, float newNearPlane, float newFarPlane);
	void Fov		(float newFov);

	//						[UTILITY]

	glm::mat4 LookAt(const glm::vec3& target);
	void Rotate(float deltaYaw, float deltaPitch);
	void Zoom(float scrollOffset);
	std::string ToString()	const;


	//						[OPERATIONS]

	friend std::ostream& operator<<(std::ostream& out, const Camera& camera);
	

private:
	glm::vec3 m_Position = glm::vec3(0.0f);
	glm::vec3 m_Front = glm::vec3(0.0f);
	glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Right = glm::vec3(0.0f);
	glm::vec3 m_LocalUp = glm::vec3(0.0f);

	// m_Fov in radians
	float m_Fov;
	float m_AspectRatio;
	float m_NearPlane;
	float m_FarPlane;

	glm::mat4 m_Projection;


	// in radians
	float m_Pitch{};
	float m_Yaw{};

	glm::mat4 Transform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation) const;
};

