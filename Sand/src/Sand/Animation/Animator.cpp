#include "sandpch.h"
#include "Animator.h"

#include "Sand/Scene/Components.h"

namespace Sand
{

	Animator::Animator()
	{
	}
	
	Animator::~Animator()
	{
	}

	void Animator::OnUpdate(Actor toUpdate, Timestep ts)
	{
		// TODO: support for 'empty' keyframes (to control lerp speed, etc)

		if (!m_Playing)
			return;

		auto& spriteRenderer = toUpdate.GetComponent<SpriteRendererComponent>();
		auto& transform = toUpdate.GetComponent<TransformComponent>();

		const Animation::Keyframe* const currentFrame = m_Animation.GetCurrentKeyframe();

		// Update color
		spriteRenderer.Color = currentFrame->Color;

		// Update transformation
		transform.Position = currentFrame->Position;
		transform.Scale = currentFrame->Scale;
		transform.Rotation = currentFrame->Rotation;

		if (m_Animation.Advance(&m_Playing, Loop, ts))
		{
			// Reset back to original keyframe
			const Animation::Keyframe* const firstKeyframe = m_Animation.GetKeyframes().data();

			spriteRenderer.Color = firstKeyframe->Color;

			transform.Position = firstKeyframe->Position;
			transform.Scale = firstKeyframe->Scale;
			transform.Rotation = firstKeyframe->Rotation;
		}
	}

	void Animator::Play()
	{
		m_Playing = true;
	}

	void Animator::Stop()
	{
		m_Playing = false;
	}

}
