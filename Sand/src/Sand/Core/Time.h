#pragma once

#include <ctime>
#include <string>

namespace Sand 
{
	class Time
	{
	public:
		// returns string containing '[hours:minutes:seconds:]'
		static std::string GetSystemTime()
		{
			time_t t; // t passed as argument in function time()
			struct tm tt; // decalring variable for localtime()
			time(&t); //passing argument to time()
			localtime_s(&tt, &t);
			std::string ret = "";
			std::string hours = std::to_string(tt.tm_hour), minutes = std::to_string(tt.tm_min), seconds = std::to_string(tt.tm_sec);
			ret = "[" + hours + ":" + minutes + ":" + seconds + "]";

			return ret;
		}
		static const float GetTime()
		{
			return m_Time;
		}


	private:
		static float m_Time;

		friend class Application;
	};

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSeconds() { return m_Time; }
		float GetMilliseconds() { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};

}