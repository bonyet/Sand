project "Box2D"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/box2d/**.h",
		"src/collision/**.cpp",
		"src/common/**.cpp",
		"src/dynamics/**.cpp",
		"src/rope/**.cpp",
		"src/collision/**.h",
		"src/common/**.h",
		"src/dynamics/**.h",
		"src/rope/**.h",
	}

	includedirs
    {
        "include"
    }

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
