#pragma once

#include <list>

namespace Sand
{

    struct GradientMark
    {
        float color[4];
        float position; // 0 to 1

        bool operator==(const GradientMark& other) const
        {
            return position == other.position &&
                color[0] == other.color[0] &&
                color[1] == other.color[1] &&
                color[2] == other.color[2] &&
                color[3] == other.color[3];
        }
        bool operator!=(const GradientMark& other) const
        {
            return !operator==(other);
        }
    };

    class ColorGradient
    {
    public:
        ColorGradient() = default;

        void AddMark(float position, float r, float g, float b, float a);
        void RemoveMark(const GradientMark& mark);
        void RefreshCache();

        void Evaluate(float position, float* color) const;

        std::list<GradientMark>& GetMarks() { return m_Marks; }
    private:
        void ComputeColorAt(float position, float* color);
    private:
        std::list<GradientMark> m_Marks;

        float m_CachedValues[256 * 4]{};
    };

}