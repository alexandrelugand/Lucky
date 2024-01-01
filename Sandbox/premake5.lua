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

	vpaths 
	{
		["Vendors/*"] = 
		{ 
			"%{includeDir.GLM}/glm/**.hpp", 
			"%{includeDir.GLM}/glm/**.inl"
		}
	}


	includedirs
	{
		"src",
		"../Lucky/src",
		"%{includeDir.spdlog}",
		"%{includeDir.GLFW}",
		"%{includeDir.GLAD}",
		"%{includeDir.ImGui}",
		"%{includeDir.GLM}",
		"%{includeDir.entt}"
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

	filter { "action:vs2022", "configurations:Debug" }
		ignoredefaultlibraries { "LIBCMT" }

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
		systemversion "latest"
		defines { "PLATFORM_WINDOWS" }

	-- Post builds
	postbuildcommands 
	{
		'{COPY} "../Vendors/GLFW/lib-static-ucrt/glfw3.dll" "%{cfg.targetdir}"',
	}