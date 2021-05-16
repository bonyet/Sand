#include "sandpch.h"
#include "Gradient.h"

#include "Sand/Math/Math.h"

namespace Sand
{
	
	void ColorGradient::AddMark(float position, float r, float g, float b, float a)
	{
		position = Math::Clamp(position, 0.0f, 1.0f);

		GradientMark mark{};
		mark.color[0] = r;
		mark.color[1] = g;
		mark.color[2] = b;
		mark.color[3] = a;
		mark.position = position;
		
		m_Marks.push_back(mark);

		RefreshCache();
	}

	void ColorGradient::RemoveMark(const GradientMark& mark)
	{
		m_Marks.remove(mark);
		RefreshCache();
	}

	void ColorGradient::Evaluate(float position, float* color) const
	{
		position = Math::Clamp(position, 0.0f, 1.0f);
		int cachePos = position * 255;
		cachePos *= 3;

		color[0] = m_CachedValues[cachePos + 0];
		color[1] = m_CachedValues[cachePos + 1];
		color[2] = m_CachedValues[cachePos + 2];
		color[3] = m_CachedValues[cachePos + 3];
	}

	void ColorGradient::ComputeColorAt(float position, float* color)
	{
		position = Math::Clamp(position, 0.0f, 1.0f);

		GradientMark* lower = nullptr, *upper = nullptr;

		for (GradientMark& gradientMarkRef : m_Marks)
		{
			GradientMark* mark = &gradientMarkRef;

			if (mark->position < position)
			{
				if (!lower || lower->position < mark->position)
				{
					lower = mark;
				}
			}

			if (mark->position >= position)
			{
				if (!upper || upper->position > mark->position)
				{
					upper = mark;
				}
			}
		}

		if (upper && !lower)
			lower = upper;
		else if (!upper && lower)
			upper = lower;
		else if (!lower && !upper)
		{
			color[0] = color[1] = color[2] = color[3] = 0.0f;
			return;
		}

		if (upper == lower)
		{
			color[0] = upper->color[0];
			color[1] = upper->color[1];
			color[2] = upper->color[2];
			color[3] = upper->color[3];
		}
		else
		{
			float distance = upper->position - lower->position;
			float delta = (position - lower->position) / distance;

			// Lerp
			color[0] = ((1.0f - delta) * lower->color[0]) + (delta * upper->color[0]);
			color[1] = ((1.0f - delta) * lower->color[1]) + (delta * upper->color[1]);
			color[2] = ((1.0f - delta) * lower->color[2]) + (delta * upper->color[2]);
			color[3] = ((1.0f - delta) * lower->color[3]) + (delta * upper->color[3]);
		}
	}

	void ColorGradient::RefreshCache()
	{
		m_Marks.sort([](const GradientMark& a, const GradientMark& b) { return a.position < b.position; });

		for (int i = 0; i < 256; ++i)
		{
			ComputeColorAt(i / 255.0f, &m_CachedValues[i * 3]);
		}
	}

}