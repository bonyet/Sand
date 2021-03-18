#include "sandpch.h"
#include "Animation.h"

#include "Sand/Scene/Components.h"

namespace Sand
{

	Animation::Animation(uint32_t fps)
		: m_FramesPerSecond(fps)
	{
	}
	
	Animation::~Animation()
	{
		m_Keyframes.clear();
	}

	static inline float FPSToSeconds(uint32_t fps)
	{
		return 1.0f / (float)fps;
	}

	bool Animation::Advance(bool* playing, bool loop, Timestep ts)
	{
		// Check to advance the animation
		m_SecondsElapsedSinceLastAdvance += ts;

		if (m_SecondsElapsedSinceLastAdvance >= FPSToSeconds(m_FramesPerSecond))
		{
			m_SecondsElapsedSinceLastAdvance = 0.0f;
			m_CurrentFrame++;
		}

		if (m_CurrentFrame >= m_Keyframes.size())
		{
			if (loop)
			{
				// Reset to beginning
				m_SecondsElapsedSinceLastAdvance = 0.0f;
				m_CurrentFrame = 0;
			}
			else
			{
				*playing = false;
			}

			return true;
		}

		return false;
	}

	void Animation::Append(const Keyframe& frame)
	{
		SAND_PROFILE_FUNCTION();

		m_Keyframes.push_back(frame);
	}
	
	void Animation::Remove(const Keyframe& frame)
	{
		SAND_PROFILE_FUNCTION();
		
		m_Keyframes.erase(std::find(m_Keyframes.begin(), m_Keyframes.end(), frame));
	}
	
	void Animation::Remove(int index)
	{
		SAND_PROFILE_FUNCTION();
		
		m_Keyframes.erase(std::find(m_Keyframes.begin(), m_Keyframes.end(), m_Keyframes.at(index)));
	}
	
	void Animation::Discard()
	{
		m_Keyframes.clear();
	}

	Animation::Keyframe* Animation::GetCurrentKeyframe()
	{
		return &m_Keyframes.data()[m_CurrentFrame];
	}

	void Animation::SetFPS(uint32_t fps)
	{
		m_FramesPerSecond = fps;
	}

}
