#pragma once

#include "Camera.h"
#include "Sand/Core/Time.h"
#include "Sand/Events/Event.h"
#include "Sand/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Sand
{

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		const glm::vec3& GetFocalPoint() const { return m_FocalPoint; }
		void SetFocalPoint(const glm::vec3& fpoint) { m_FocalPoint = fpoint; }

		float GetDistance() const { return m_Distance; }
		void SetDistance(float distance) { m_Distance = distance; }
		
		void SetVerticalFOV(float fov) { m_FOV = fov; UpdateProjection(); }
		void SetNearClip(float nearclip) { m_NearClip = nearclip; UpdateProjection(); }
		void SetFarClip(float farclip) { m_FarClip = farclip; UpdateProjection(); }

		void SetRotation(float pitch, float yaw) { m_Pitch = pitch, m_Yaw = yaw; }

		void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		bool Use2DControls = true;
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}