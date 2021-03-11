#include "sandpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Sand
{

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(float width, float height)
	{
		mAspectRatio = width / height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = mOrthographicSize * mAspectRatio * 0.5f;
		float orthoRight = -mOrthographicSize * mAspectRatio * 0.5f;
		float orthoBottom = -mOrthographicSize * 0.5f;
		float orthoTop = mOrthographicSize * 0.5f;

		m_Projection = glm::ortho(orthoRight, orthoLeft, orthoBottom, orthoTop, mOrthographicNear, mOrthographicFar);
	}

}