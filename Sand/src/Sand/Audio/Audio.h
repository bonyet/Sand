#pragma once

#include "AudioClip.h"

namespace Sand
{

	class AudioSource
	{
	public:
		AudioSource() = default;
		AudioSource(uint32_t handle, float lengthInSeconds);
		~AudioSource();

		void Play();
		void Pause();
		void Stop();

		void SetPosition(float x, float y, float z);
		void SetSpatial(bool spatial);
		void SetGain(float gain);
		void SetPitch(float pitch);

		float* GetPosition() { return m_Position; }
		bool IsSpatial() const { return m_Spatial; }
		float GetGain()  const { return m_Gain; }
		float GetPitch() const { return m_Pitch; }

		static AudioSource LoadFromFile(const std::string& filepath);
	private:
		uint32_t m_Handle;
		bool m_Spatial = false;
		float m_LengthInSeconds = 0.0f, m_Gain = 0.5f, m_Pitch = 1.0f;
		float m_Position[3] = { 0.0f, 0.0f, 0.0f };

		friend class Audio;
	};

	class AudioListener
	{
	public:
		AudioListener();

		static AudioListener* GetInstance();
	private:
		void Init();
	private:
		static AudioListener* s_Instance;
	};

	class Audio
	{
	public:
		static void Init();
		static void Shutdown();

		static AudioSource LoadAudioSource(const AudioClip& audioClip);
		static AudioSource LoadAudioSource(const std::string& filepath);

		static void Lock();
		static void Unlock();
		static bool IsUnlocked();
	};

}