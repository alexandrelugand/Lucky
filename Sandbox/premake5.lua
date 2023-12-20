project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
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
		"GLAD",
		"ImGui"
	}

	-- Precompile header
	pchheader "SandboxPch.h"
	pchsource "src/SandboxPch.cpp"

	filter "action:gmake2"
		buildoptions { "-Wall" }
		links { "glfw3" }
		-- Using ccache to accelerate compilation time (not mandatory)
		-- makesettings [[CXX = ccache g++]]

	filter "action:vs2022"
		defines { "_CRT_SECURE_NO_WARNINGS" }
		disablewarnings { "4996" }
		ignoredefaultlibraries { "LIBCMT" }
		links { "glfw3_mt" }

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