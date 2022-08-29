#pragma once
#include <glm/glm.hpp>
#include <string>
#include <iostream>

class Camera
{
public:
	//						[CONSTRUCTORS]

	Camera(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float fov, float aspectRatio, float near, float far);
	Camera() = delete;
	Camera(const Camera& camera) = delete;


	//						[GETTERS]

	const glm::vec3& Position()		const;
	const glm::vec3& Rotation()		const;
	const glm::vec3& Scale()		const;
	const glm::mat4& View()			const;
	const glm::mat4& Projection()	const;
	float			 Fov()			const;
	float			 Aspect()		const;
	float			 NearPlane()	const;
	float			 FarPlane()		const;


	//						[SETTERS]

	void Position	(const glm::vec3& newPosition);
	void Rotation	(const glm::vec3& newRotation);
	void Projection	(float newFov, float newAspectRatio, float newNearPlane, float newFarPlane);


	//						[UTILITY]

	std::string ToString()	const;


	//						[OPERATIONS]

	friend std::ostream& operator<<(std::ostream& out, const Camera& camera);
	

private:
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);

	float m_Fov;
	float m_AspectRatio;
	float m_NearPlane;
	float m_FarPlane;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	glm::mat4 Transform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation) const;
};

