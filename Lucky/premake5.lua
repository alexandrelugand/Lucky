project "Lucky"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
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
		"%{includeDir.GLM}/glm/**.inl",
		"%{includeDir.stb}/**.h", 
		"%{includeDir.stb}/**.cpp", 
	}

	includedirs
	{
		"src",
		"%{includeDir.spdlog}",
		"%{includeDir.GLFW}",
		"%{includeDir.GLAD}",
		"%{includeDir.ImGui}",
		"%{includeDir.ImGui}/backends",
		"%{includeDir.GLM}",
		"%{includeDir.stb}"
	}

	libdirs 
	{
		"%{libDir.GLFW}"
	}

	links
	{
		"opengl32",
		"GLAD",
		"ImGui"
	}

	defines
	{
		"GLFW_INCLUDE_NONE"
	}

	-- Precompile header
	pchheader "LuckyPch.h"
	pchsource "src/LuckyPch.cpp"

	-- G++
	filter "action:gmake2"
		buildoptions { "-Wall" }
		links { "glfw3" }
		-- Using ccache to accelerate compilation time (not mandatory)
		-- makesettings [[CXX = ccache g++]]

	filter { "action:gmake2", "configurations:Release"}
		buildoptions { "-O3" }

	-- VS 2022
	filter "action:vs2022"
		defines { "_CRT_SECURE_NO_WARNINGS" }
		disablewarnings { "4996" }
		links { "glfw3_mt" }

	-- Configurations
	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On" 

	filter "configurations:Release"
		defines { "NDEBUG" }
		runtime "Release"
		optimize "On" 

	-- Platforms
	filter "system:windows"
		defines { "PLATFORM_WINDOWS" }
