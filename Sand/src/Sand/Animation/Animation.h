#pragma once

#include "Sand/Core/Time.h"

#include <glm/glm.hpp>
#include <vector>

namespace Sand
{

	class Actor;

	class Animation
	{
	public:
		struct Keyframe
		{
			glm::vec2 Position, Scale;
			float Rotation;

			glm::vec4 Color;

			Keyframe(const glm::vec2& pos, const glm::vec2& scale, float rotation, const glm::vec4& color)
				: Position(pos), Scale(scale), Rotation(rotation), Color(color) { }

			bool operator==(const Keyframe& other)
			{
				return Rotation == other.Rotation &&
					Position == other.Position &&
					Scale == other.Scale &&
					Color == other.Color;
			}
		};
	public:
		Animation(uint32_t fps = 0);
		~Animation();

		bool Advance(bool* playing, bool loop, Timestep ts);

		void Append(const Keyframe& frame);
		template<typename... Args>
		void Append(Args&&... args)
		{
			m_Keyframes.emplace_back(std::forward<Args>(args)...);
		}

		void Remove(const Keyframe& frame);
		void Remove(int index);

		void Discard();

		Keyframe* GetCurrentKeyframe();
		const std::vector<Keyframe>& GetKeyframes() const { return m_Keyframes; }

		uint32_t GetFPS() const  { return m_FramesPerSecond; }
		void SetFPS(uint32_t fps);
	private:
		float m_SecondsElapsedSinceLastAdvance = 0.0f;
		uint32_t m_FramesPerSecond = 0;
		uint32_t m_CurrentFrame = 0;

		std::vector<Keyframe> m_Keyframes;
	private:
		friend class Animator;
	};

}