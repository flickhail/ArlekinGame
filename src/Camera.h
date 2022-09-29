#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Camera
{
public:
	//				[CONSTRUCTORS]

	Camera() = default;
	Camera(const glm::vec3& pos, float nearPlane, float farPlane) noexcept;
	virtual ~Camera() = default;


	//				[GETTERS]

	const glm::mat4& View()			const noexcept { return m_View; }
	const glm::mat4& Projection() 	const noexcept { return m_Projection; }
	float			 NearPlane() 	const noexcept { return m_NearPlane; }
	float			 FarPlane() 	const noexcept { return m_FarPlane; }

	const glm::vec3& Position()		const noexcept { return m_Position; }
	const glm::vec3& Front()		const noexcept { return m_FrontVec; }
	const glm::vec3& Right()		const noexcept { return m_RightVec; }
	const glm::vec3& Up()			const noexcept { return m_LocalUpVec; }

	// Gets the speed value (distance per frame)
	float Speed() const noexcept { return m_Speed; }
	float Scale() const noexcept { return m_Scale; }


	//				[SETTERS]

	virtual void Position(const glm::vec3& newPosition) = 0;
	virtual void ClipDistance(float newNearPlane, float newFarPlane) = 0;

	// Changes the scale of the world
	virtual float Scale(float newScale) = 0;

	// Sets the speed value (distance per frame)
	float Speed(float newSpeed);


	//				[UTILITY]

	virtual void Move(const glm::vec3& movementVector) = 0;
	virtual std::string ToString() const;
	//virtual void 


	//				[OPERATIONS]

	friend std::ostream& operator<<(std::ostream& out, const Camera& camera);

protected:
	glm::vec3 m_Position	{};
	glm::vec3 m_FrontVec	{ 0.0f, 0.0f, -1.0f };
	glm::vec3 m_RightVec	{ 1.0f, 0.0f, 0.0f };
	glm::vec3 m_WorldUpVec	{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_LocalUpVec	{ 0.0f, 1.0f, 0.0f };
	glm::mat4 m_View{};
	glm::mat4 m_Projection{};

	float m_Scale{ 1.0f };
	float m_Speed{ 1.0f };

	float m_NearPlane{};
	float m_FarPlane{};


	//				[UTILITY]

	static glm::mat4 Transform(const glm::vec3& pos, const glm::vec3& scale = { 1.0f, 1.0f, 1.0f }, const glm::vec3& rotation = { 0.0f, 0.0f, 0.0f }) noexcept;
	virtual void UpdateViewMatrix() = 0;
	virtual void UpdateProjectionMatrix() = 0;
};