#pragma once

namespace Sand 
{

	enum class Mousecode : uint16_t
	{
		Left   = 0,
		Right  = 1,
		Middle = 2,
	};

}

#define SAND_MOUSE_BUTTON_LEFT   Sand::Mousecode::Left
#define SAND_MOUSE_BUTTON_RIGHT  Sand::Mousecode::Right
#define SAND_MOUSE_BUTTON_MIDDLE Sand::Mousecode::Middle
