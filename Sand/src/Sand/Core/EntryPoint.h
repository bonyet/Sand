#pragma once

#include "Sand/Core/Base.h"

#ifdef SAND_PLATFORM_WINDOWS

extern Sand::Application* Sand::CreateApplication();

int main(int argc, char** argv)
{
	Sand::Log::Init();
	
	SAND_PROFILE_BEGIN_SESSION("Startup", "SandProfile-Startup.json");
	auto app = Sand::CreateApplication();
	SAND_PROFILE_END_SESSION();

	SAND_PROFILE_BEGIN_SESSION("Runtime", "SandProfile-Runtime.json");
	app->Run();	
	SAND_PROFILE_END_SESSION();

	SAND_PROFILE_BEGIN_SESSION("Startup", "SandProfile-Shutdown.json");
	delete app;
	SAND_PROFILE_END_SESSION();
}

#endif
