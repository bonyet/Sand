#pragma once

#include "Sand/Renderer/Camera.h"

namespace Sand
{

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetViewportSize(float width, float height);

		float GetSize() const { return m_Size; }
		void SetSize(float size) { m_Size = size; RecalculateProjection(); }
		float GetNearClip() const { return m_Near; }
		void SetNearClip(float clip) { m_Near = clip; RecalculateProjection(); }
		float GetFarClip() const { return m_Far; }
		void SetFarClip(float clip) { m_Far = clip; RecalculateProjection(); }

		float GetAspectRatio() const { return m_AspectRatio; }

		void ScreenToWorldPoint(glm::vec2& worldPoint, const glm::mat4& view);
		void WorldToScreenPoint(glm::vec2& worldPoint, const glm::mat4& view);
	private:
		void RecalculateProjection();
	private:
		float m_Size = 10.0f;
		float m_Near = -1.0f, m_Far = 1.0f;

		float m_AspectRatio = 0.0f;

		friend class EditorCamera;
	};

}