project "App"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	architecture "x64"
	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. tmpdir)
	
	dependson { "GLAD", "ImGui", "Lucky" }

	files { "**.h", "**.cpp" }

	includedirs
	{
		"src",
		"../Lucky/src",
		"%{includeDir.spdlog}",
		"%{includeDir.GLFW}",
		"%{includeDir.GLAD}",
		"%{includeDir.ImGui}"
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

	filter "configurations:Debug"
		defines { "DEBUG", "SDL_MAIN_HANDLED" }
		symbols "On" 

	filter "configurations:Release"  
		defines { "NDEBUG", "SDL_MAIN_HANDLED" }
		optimize "On" 

	filter "system:windows"
		systemversion "latest"

		defines { "PLATFORM_WINDOWS" }

		postbuildcommands 
		{
			'{COPY} "../Vendors/GLFW/lib-static-ucrt/glfw3.dll" "%{cfg.targetdir}"',
		}