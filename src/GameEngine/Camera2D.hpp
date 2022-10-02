#pragma once
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GameEngine
{
	class Camera2D final : public Camera
	{
	public:
		//				[CONSTRUCTORS]

		Camera2D() = default;
		Camera2D(const glm::vec3& position, float aspectRatio, float nearPlane, float farPlane);


		//				[GETTERS]

		const glm::vec2& Scale() const noexcept { return m_Scale; }


		//				[SETTERS]

		void Position(const glm::vec3& newPosition) override;
		void ClipDistance(float newNearPlane, float newFarPlane) override;
		void AspectRatio(float newAspectRatio);
		glm::vec2 Scale(const glm::vec2& newScale);



		//				[UTILITY]

		void Move(const glm::vec3& moveVector) override;
		std::string ToString() const override;

	private:
		float m_AspectRatio{};
		glm::vec2 m_Scale{ 1.0f };


		//				[UTILITY]

		// Recalculates the view matrix of the camera
		void UpdateViewMatrix() override;
		void UpdateProjectionMatrix() override;
	};
}
