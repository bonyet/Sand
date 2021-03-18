#pragma once

#include "Sand/Scene/Actor.h"
#include "Animation.h"

namespace Sand
{

	class Animator
	{
	public:
		Animator();
		~Animator();

		void SetAnimation(const Animation& animation) { m_Animation = animation; }
		Animation& GetAnimation() { return m_Animation; }

		void OnUpdate(Actor toUpdate, Timestep ts);

		void Play();
		void Stop();
	public:
		bool Loop = false;
	private:
		Animation m_Animation;
		bool m_Playing = false;
	};

}