#include "sandpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Sand/Core/Application.h"

namespace Sand
{

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(float width, float height)
	{
		m_AspectRatio = width / height;
		RecalculateProjection();
	}

	glm::vec2 SceneCamera::ScreenToWorldPoint(glm::vec2& screenPoint, const glm::mat4& view)
	{
		Window& window = Application::Get().GetWindow();
		float width = (float)window.GetWidth();
		float height = (float)window.GetHeight();

		glm::vec4 v4Point = { screenPoint, 1.0f, 1.0f };

		glm::mat4 invProjView = glm::inverse(m_Projection * view);

		// mouse NDC
		v4Point.x = (2.0f * ((float)(v4Point.x) / (width))) - 1.0f;
		v4Point.y = 1.0f - (2.0f * ((float)(v4Point.y) / (height)));

		v4Point = v4Point * invProjView;
		v4Point /= v4Point.w;

		return v4Point;
	}

	glm::vec2 SceneCamera::WorldToScreenPoint(glm::vec2& worldPoint, const glm::mat4& view)
	{
		Window& window = Application::Get().GetWindow();
		float width = (float)window.GetWidth();
		float height = (float)window.GetHeight();

		glm::vec4 v4Point = { worldPoint, 1.0f, 1.0f };

		glm::mat4 invProjView = glm::inverse(m_Projection * view);

		// mouse NDC
		v4Point.x = (2.0f * ((float)(v4Point.x) / (width))) - 1.0f;
		v4Point.y = 1.0f - (2.0f * ((float)(v4Point.y) / (height)));

		v4Point = v4Point / invProjView;
		v4Point /= v4Point.w;

		return v4Point;
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = m_Size * m_AspectRatio * 0.5f;
		float orthoRight = -m_Size * m_AspectRatio * 0.5f;
		float orthoBottom = -m_Size * 0.5f;
		float orthoTop = m_Size * 0.5f;

		m_Projection = glm::ortho(orthoRight, orthoLeft, orthoBottom, orthoTop, m_Near, m_Far);
	}

}