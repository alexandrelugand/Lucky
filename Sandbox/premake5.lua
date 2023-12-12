project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	architecture "x64"
	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. tmpdir)
	staticruntime "on"

	dependson { "GLAD", "ImGui", "Lucky" }

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
		"../Lucky/src",
		"%{includeDir.spdlog}",
		"%{includeDir.GLFW}",
		"%{includeDir.GLAD}",
		"%{includeDir.ImGui}",
		"%{includeDir.GLM}"
	}

	libdirs 
	{
		"%{libDir.GLFW}"
	}

	links
	{
		"Lucky",
		"opengl32",
		"GLFW3",
		"GLAD",
		"ImGui"
	}

	-- Precompile header
	pchheader "SandboxPch.h"
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
		systemversion "latest"

		defines { "PLATFORM_WINDOWS" }

		postbuildcommands 
		{
			'{COPY} "../Vendors/GLFW/lib-static-ucrt/glfw3.dll" "%{cfg.targetdir}"',
		}