project "Lucky"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	architecture "x64"
	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. tmpdir)

	dependson { "GLAD", "ImGui" }

	files { "**.h", "**.cpp" }

	includedirs
	{
		"src",
		"%{includeDir.spdlog}",
		"%{includeDir.GLFW}",
		"%{includeDir.GLAD}",
		"%{includeDir.ImGui}",
		"%{includeDir.ImGui}/backends"
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

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On" 

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On" 

	filter "system:windows"
		defines { "PLATFORM_WINDOW" }
