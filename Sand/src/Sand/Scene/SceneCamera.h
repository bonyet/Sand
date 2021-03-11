#pragma once

#include "Sand/Renderer/Camera.h"

namespace Sand
{

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(float width, float height);

		float GetOrthographicSize() const { return mOrthographicSize; }
		void SetOrthographicSize(float size) { mOrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return mOrthographicNear; }
		void SetOrthographicNearClip(float clip) { mOrthographicNear = clip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return mOrthographicFar; }
		void SetOrthographicFarClip(float clip) { mOrthographicFar = clip; RecalculateProjection(); }

		float GetAspectRatio() const { return mAspectRatio; }
	private:
		void RecalculateProjection();
	private:
		float mOrthographicSize = 10.0f;
		float mOrthographicNear = -1.0f, mOrthographicFar = 1.0f;

		float mAspectRatio = 0.0f;

		friend class EditorCamera;
	};

}