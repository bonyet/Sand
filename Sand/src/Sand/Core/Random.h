#pragma once

#include <random>
#include <chrono>

namespace Sand
{

	class Random
	{
	private:
		std::mt19937 m_Re;
		bool AutomaticallyReseed = true;
		__int64 m_Seed;
	public:
		void Seed(int seed)
		{
			m_Re.seed(seed);
		}

		// Greater or equal to min, less than max
		int RandomInt(int min, int max)
		{
			if (AutomaticallyReseed)
			{
				m_Seed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

				m_Re.seed(static_cast<unsigned int>(m_Seed));
			}

			std::uniform_int_distribution<int> unif(min, max);

			return unif(m_Re);
		}

		// Greater or equal to min, less than max
		double RandomDouble(double min, double max)
		{
			if (AutomaticallyReseed)
			{
				m_Seed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

				m_Re.seed(static_cast<unsigned int>(m_Seed));
			}

			std::uniform_real_distribution<double> unif(min, max);

			return unif(m_Re);
		}
	};

}