project "Lucky"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	architecture "x64"
	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. tmpdir)
	staticruntime "on"

	dependson { "GLAD", "ImGui" }

	files
	{ 
		"**.h", 
		"**.cpp", 
		"%{includeDir.GLM}/glm/**.hpp", 
		"%{includeDir.GLM}/glm/**.inl" 
	}

	includedirs
	{
		"src",
		"%{includeDir.spdlog}",
		"%{includeDir.GLFW}",
		"%{includeDir.GLAD}",
		"%{includeDir.ImGui}",
		"%{includeDir.ImGui}/backends",
		"%{includeDir.GLM}"
	}

	libdirs 
	{
		"%{libDir.GLFW}"
	}

	links
	{
		"opengl32",
		"GLFW3",
		"GLAD",
		"ImGui"
	}

	defines
	{
		"GLFW_INCLUDE_NONE"
	}

	-- Precompile header
	pchheader "LuckyPch.h"
	-- Using ccache to accelerate compilation time (not mandatory)
	makesettings [[CXX = ccache g++]]

	filter { "action:gmake2" }
  		buildoptions { "-Wall" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On" 

	filter "configurations:Release"
		defines { "NDEBUG" }
		runtime "Release"
		optimize "On" 

	filter "system:windows"
		defines { "PLATFORM_WINDOW" }
