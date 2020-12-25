#include "sandpch.h"
#include "Sand/Core/Window.h"

#ifdef SAND_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Sand
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef SAND_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		SAND_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}

}