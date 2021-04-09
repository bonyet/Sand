#include "sandpch.h"

#include "Audio.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define MINIMP3_IMPLEMENTATION
#include <minimp3/minimp3.h>
#include <minimp3/minimp3_ex.h>

#include <iostream>
#include <cassert>

#include "alHelpers.h"

namespace Sand
{
	static mp3dec_t s_Mp3d;
	static ALCdevice* s_AudioDevice = nullptr;
	AudioListener* AudioListener::s_Instance;

	static bool s_AudioLocked = false;

	AudioSource::AudioSource(uint32_t handle, float lengthInSeconds)
		: m_Handle(handle), m_LengthInSeconds(lengthInSeconds)
	{
	}

	AudioSource::~AudioSource()
	{
	}

	void AudioSource::Play()
	{
		if (s_AudioLocked)
			return; // Don't play noises if the audio is locked (scene in edit mode)

		if (!m_Handle) {
			SAND_CORE_ERROR("[OpenAL Error]: Invalid audio source handle '{0}'.", m_Handle);
			return;
		}

		alSourcePlay(m_Handle);
	}

	void AudioSource::Pause()
	{
		if (!m_Handle) {
			SAND_CORE_ERROR("[OpenAL Error]: Invalid audio source handle '{0}'.", m_Handle);
			return;
		}

		alSourcePause(m_Handle);
	}

	void AudioSource::Stop()
	{
		if (!m_Handle) {
			SAND_CORE_ERROR("[OpenAL Error]: Invalid audio source handle '{0}'.", m_Handle);
			return;
		}

		alSourceStop(m_Handle);
	}

	void AudioSource::SetPosition(float x, float y, float z)
	{
		m_Position[0] = x;
		m_Position[1] = y;
		m_Position[2] = z;

		if (!m_Handle) {
			SAND_CORE_ERROR("[OpenAL Error]: Invalid audio source handle '{0}'.", m_Handle);
			return;
		}

		alSource3f(m_Handle, AL_POSITION, x, y, z);
	}
	void AudioSource::SetSpatial(bool spatial)
	{
		m_Spatial = spatial;

		if (!m_Handle) {
			SAND_CORE_ERROR("[OpenAL Error]: Invalid audio source handle '{0}'.", m_Handle);
			return;
		}

		alSourcei(m_Handle, AL_SOURCE_SPATIALIZE_SOFT, spatial);
		alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	}
	void AudioSource::SetGain(float gain)
	{
		m_Gain = gain;

		if (!m_Handle) {
			SAND_CORE_ERROR("[OpenAL Error]: Invalid audio source handle '{0}'.", m_Handle);
			return;
		}

		alSourcef(m_Handle, AL_GAIN, gain);
	}

	void AudioSource::SetPitch(float pitch)
	{
		m_Pitch = pitch;

		if (!m_Handle) {
			SAND_CORE_ERROR("[OpenAL Error]: Invalid audio source handle '{0}'.", m_Handle);
			return;
		}

		alSourcef(m_Handle, AL_PITCH, pitch);
	}

	AudioSource AudioSource::LoadFromFile(const std::string& filepath)
	{
		return Audio::LoadAudioSource(filepath);
	}

	void Audio::Init()
	{
		if (InitAL(s_AudioDevice, nullptr, 0) != 0)
		{
			std::cout << "Failed to initialize OpenAL device." << std::endl;
			assert(false && "Failed to initialize OpenAL device.");
		}

		mp3dec_init(&s_Mp3d);
	}

	void Audio::Shutdown()
	{
		CloseAL();
	}

	static ALenum GetFormat(int channels)
	{
		switch (channels)
		{
		case 1: return AL_FORMAT_MONO16;
		case 2: return AL_FORMAT_STEREO16;
		}

		assert(false);
		return 0;
	}

	AudioSource Audio::LoadAudioSource(const AudioClip& audioClip)
	{
		return LoadAudioSource(audioClip.Filepath);
	}

	AudioSource Audio::LoadAudioSource(const std::string& filepath)
	{
		mp3dec_file_info_t info;
		if (mp3dec_load(&s_Mp3d, "testSound.mp3", &info, NULL, NULL))
		{
			std::cout << "Failed to load file." << std::endl;
			assert(false && "Failed to load file.");
		}
		uint32_t size = (uint32_t)info.samples * sizeof(mp3d_sample_t);

		int sampleRate = info.hz;
		int channels = info.channels;
		auto format = GetFormat(channels);
		float lengthInSeconds = size / (info.avg_bitrate_kbps * 1024.0f);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, info.buffer, size, sampleRate);

		AudioSource audioSource = { buffer, lengthInSeconds };

		ALuint source;
		alGenSources(1, &source);
		alSourcef(source, AL_PITCH, 1);
		alSourcef(source, AL_GAIN, 1.0f);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		alSourcei(source, AL_LOOPING, AL_FALSE);
		alSourcei(source, AL_BUFFER, buffer);

		return audioSource;
	}

	void Audio::Lock()
	{
		s_AudioLocked = true;
	}

	void Audio::Unlock()
	{
		s_AudioLocked = false;
	}

	bool Audio::IsUnlocked()
	{
		return s_AudioLocked;
	}

	AudioListener::AudioListener()
	{
		if (!s_Instance)
		{
			s_Instance = this;
		} else
		{
			SAND_CORE_WARN("Audio listener already exists. There can only be one audio listener active at a time.");
		}
	}

	AudioListener* AudioListener::GetInstance()
	{
		return s_Instance;
	}

	void AudioListener::Init()
	{
		float position[] = { 0.0f, 0.0f, 0.0f };
		float velocity[] = { 0.0f, 0.0f, 0.0f };
		float orientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

		alListenerfv(AL_POSITION, position);
		alListenerfv(AL_VELOCITY, velocity);
		alListenerfv(AL_ORIENTATION, orientation);
	}

}