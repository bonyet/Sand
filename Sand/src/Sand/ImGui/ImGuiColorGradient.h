// Created by David Gallardo on 11/06/16.

#pragma once

#include "Sand/Utils/Gradient.h"

namespace ImGui
{

    bool GradientButton(Sand::ColorGradient* gradient);
    bool GradientEditor(Sand::ColorGradient* gradient, Sand::GradientMark*& draggingMark, Sand::GradientMark*& selectedMark);

}