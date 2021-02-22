workspace "Sand"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]      = "Sand/vendor/GLFW/include"
IncludeDir["Glad"]      = "Sand/vendor/Glad/include"
IncludeDir["ImGui"]     = "Sand/vendor/imgui"
IncludeDir["glm"]       = "Sand/vendor/glm"
IncludeDir["stb_image"] = "Sand/vendor/stb_image"
IncludeDir["entt"]      = "Sand/vendor/entt/include"
IncludeDir["yaml_cpp"]  = "Sand/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"]  = "Sand/vendor/ImGuizmo"
IncludeDir["Box2D"]     = "Sand/vendor/box2d/include"
IncludeDir["Mono"]      = "Sand/vendor/Mono/include/mono-2.0"

LibraryDir = {}
LibraryDir["Mono"] = "%{wks.location}/Sand/vendor/Mono/lib/mono-2.0-sgen.lib"

group "Dependencies"
	include "Sand/vendor/GLFW"
	include "Sand/vendor/Glad"
	include "Sand/vendor/imgui"
	include "Sand/vendor/yaml-cpp"
	include "Sand/vendor/box2d"
group ""

project "Sand"
	location "Sand"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "sandpch.h"
	pchsource "Sand/src/sandpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",

		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.Mono}",
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"Box2D",
		"%{LibraryDir.Mono}",
		"opengl32.lib",
	}

	filter "files:Sand/vendor/ImGuizmo/**.cpp"
		flags { "NoPch" }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "SAND_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SAND_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SAND_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Sand/vendor/spdlog/include",
		"Sand/src",
		"Sand/vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Box2D}",
	}

	links
	{
		"Sand"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "SAND_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SAND_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SAND_DIST"
		runtime "Release"
		optimize "on"

project "Sand-Editor"
		location "Sand-Editor"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
	
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}
	
		includedirs
		{
			"Sand/vendor/spdlog/include",
			"Sand/src",
			"Sand/vendor",
			"%{IncludeDir.entt}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.ImGuizmo}",
			"%{IncludeDir.Box2D}",
		}
	
		links
		{
			"Sand"
		}
	
		filter "system:windows"
			systemversion "latest"
			
		filter "configurations:Debug"
			defines "SAND_DEBUG"
			runtime "Debug"
			symbols "on"
	
		filter "configurations:Release"
			defines "SAND_RELEASE"
			runtime "Release"
			optimize "on"
	
		filter "configurations:Dist"
			defines "SAND_DIST"
			runtime "Release"
			optimize "on"


project "Sand-CSClient"
		location "Sand-CSClient"
		kind "SharedLib"
		language "C#"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files 
		{
			"%{prj.name}/src/**.cs", 
		}