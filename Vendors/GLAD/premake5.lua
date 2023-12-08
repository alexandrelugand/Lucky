project "GLAD"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	architecture "x64"
	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. tmpdir)

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c",
	} 

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On" 

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On" 