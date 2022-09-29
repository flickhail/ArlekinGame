#pragma once
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D final : public Camera
{
public:
	//				[CONSTRUCTORS]

	Camera2D() = default;
	Camera2D(const glm::vec3& position, float width, float height, float nearPlane, float farPlane);


	//				[SETTERS]

	void Position(const glm::vec3& newPosition) override;
	void ClipDistance(float newNearPlane, float newFarPlane) override;
	float Scale(float newScale) override;


	//				[UTILITY]

	void Move(const glm::vec3& moveVector) override;
	std::string ToString() const override;

private:
	float m_Width{};
	float m_Height{};


	//				[UTILITY]

	// Recalculates the view matrix of the camera
	void UpdateViewMatrix() override;
	void UpdateProjectionMatrix() override;
};