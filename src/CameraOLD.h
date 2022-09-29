#pragma once
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <stdexcept>

class CameraOLD
{
public:
	struct Settings2D
	{
		Settings2D(int width, int height) : Width(width), Height(height) { }
		int Width{};
		int Height{};
	};

	// The camera settings that represent all 3D properties of the camera.
	// Use this struct if you want to create the 3D camera
	struct Settings3D
	{
		Settings3D(float fov, float aspectRatio) : FOV(fov), AspectRatio(aspectRatio) { }
		float FOV{};
		float AspectRatio{};
	};

	enum class ProjectionMode
	{
		Undefined,
		Orthographic,
		Perspective
	};

	//						[CONSTRUCTORS]

	CameraOLD();
	CameraOLD(const glm::vec3& position, const Settings2D& settings, float nearPlane = 0.1f, float farPlane = 1000.0f);
	CameraOLD(const glm::vec3& position, const Settings3D& settings, float nearPlane = 0.1f, float farPlane = 1000.0f);


	//						[GETTERS]

	const glm::mat4& View()			const;
	const glm::mat4& Projection()	const;
	float			 FOV()			const;
	float			 Aspect()		const;
	float			 NearPlane()	const;
	float			 FarPlane()		const;

	const glm::vec3& Position()		const;
	const glm::vec3& Front()		const;
	const glm::vec3& Right()		const;
	const glm::vec3& Up()			const;


	//						[SETTERS]

	void Position		(const glm::vec3& newPosition);
	void Orthographic	(int newWidth, int newHeight);
	void Perspective	(const Settings3D& newSettings, float nearPlane, float farPlane);
	void FOV			(float newFov);
	void Aspect			(float newWidth, float newHeight);
	void ClipDistance	(float newNearPlane, float newFarPlane);


	//						[UTILITY]

	void LookAt(const glm::vec3& target);
	void Transform2D(const glm::vec3& translation, float zRotation = 0.0f);
	void Transform3D(const glm::vec3& translation, const glm::vec3& rotation = glm::vec3{ 0.0f });
	void Rotate(float deltaYaw, float deltaPitch);
	void Zoom(float scrollOffset);
	std::string ToString()	const;


	//						[OPERATIONS]

	friend std::ostream& operator<<(std::ostream& out, const CameraOLD& camera);
	

private:
	//			[GENERAL CAMERA MEMBERS]

	glm::vec3 m_Position =	{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Front =		{ 0.0f, 0.0f, -1.0f };
	glm::vec3 m_Right =		{ 1.0f, 0.0f, 0.0f };
	glm::vec3 m_WorldUp =	{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_LocalUp =	{ 0.0f, 1.0f, 0.0f };
	float m_NearPlane{};
	float m_FarPlane{};
	ProjectionMode m_ProjectionMode{ ProjectionMode::Undefined };
	glm::mat4 m_View{};


	//			[2D MODE MEMBERS]

	// In pixels
	int m_Width{};

	// In pixels
	int m_Height{};
	glm::mat4 m_Orthographic{};


	//			[3D MODE MEMBERS]

	// In radians
	float m_FOV{};
	float m_AspectRatio{};

	// In radians
	float m_Pitch{};

	// In radians
	float m_Yaw{};

	glm::mat4 m_Perspective{};

	//			[UTILITY]

	// Only calculates the transform matrix. Functionally pure
	glm::mat4 Transform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation) const;
};

