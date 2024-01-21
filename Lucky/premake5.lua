project "Lucky"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	architecture "x64"
	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. tmpdir)
	staticruntime "off"

	dependson { "GLAD", "ImGui", "yaml-cpp" }

	files
	{ 
		"**.h",
		"**.cpp",
		-- "src/*.h", 
		-- "src/*.cpp", 
		-- "src/Lucky/*.h", 
		-- "src/Lucky/*.cpp", 
		-- "src/Lucky/Application/**.h",
		-- "src/Lucky/Application/**.cpp",
		-- "src/Lucky/Core/**.h",
		-- "src/Lucky/Core/**.cpp",
		-- "src/Lucky/Math/**.h",
		-- "src/Lucky/Math/**.cpp",
		-- "src/Lucky/Renderer/**.h",
		-- "src/Lucky/Renderer/**.cpp",
		-- "src/Lucky/Scene/**.h",
		-- "src/Lucky/Scene/**.cpp",
		"%{includeDir.GLM}/glm/**.hpp", 
		"%{includeDir.GLM}/glm/**.inl",
		"%{includeDir.stb}/**.h", 
		"%{includeDir.stb}/**.cpp"
	}

	vpaths 
	{ 
		["Vendors/*"] = 
		{ 
			"%{includeDir.GLM}/glm/**.hpp", 
			"%{includeDir.GLM}/glm/**.inl",
			"%{includeDir.stb}/**.h",
			"%{includeDir.stb}/**.cpp"
		} 
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
		"%{includeDir.stb}",
		"%{includeDir.entt}",
		"%{includeDir.yaml}",
		"%{includeDir.ImGuizmo}",
		"%{includeDir.nameof}"
	}

	libdirs 
	{
		"%{libDir.GLFW}"
	}

	links
	{
		"opengl32",
		"GLAD",
		"ImGui",
		"ImGuizmo",
		"yaml-cpp"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}

	-- Precompile header
	pchheader "LuckyPch.h"
	pchsource "src/LuckyPch.cpp"

	filter "files:../Vendors/ImGuizmo/*.cpp"
		flags { "NoPCH" }

	-- Configurations
	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On" 
		optimize "Off"

	filter "configurations:Release"
		defines { "NDEBUG" }
		runtime "Release"
		optimize "full"

	-- EMScripten
	filter { "action:gmake2" }
		architecture "x86"
		makesettings [[
CC = emcc
CXX = em++
AR = emar
		]]
		targetextension  ".a"
		buildoptions { "-Wall -Wformat -s DISABLE_EXCEPTION_CATCHING=1 -Wno-deprecated-include-gch" }
		linkoptions { "-s DISABLE_EXCEPTION_CATCHING=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_GLFW=3 -s FULL_ES3=1 -s FORCE_FILESYSTEM=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 --use-preload-plugins --no-heap-copy" }
		excludes
		{
			"src/Lucky/Platforms/Windows/**.h",
			"src/Lucky/Platforms/Windows/**.cpp",
			"src/Lucky/Platforms/OpenGL/**.h",
			"src/Lucky/Platforms/OpenGL/**.cpp",
		}
		-- files
		-- {
		-- 	"src/Lucky/Platforms/Browser/**.h",
		-- 	"src/Lucky/Platforms/Browser/**.cpp",
		-- 	"src/Lucky/Platforms/OpenGLES3/**.h",
		-- 	"src/Lucky/Platforms/OpenGLES3/**.cpp",
		-- }

	-- VS 2022
	filter "action:vs2022"
		defines { "PLATFORM_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
		disablewarnings { "4996" }
		links { "glfw3_mt" }
		excludes
		{
			"src/Lucky/Platforms/Browser/**.h",
			"src/Lucky/Platforms/Browser/**.cpp",
			"src/Lucky/Platforms/OpenGLES3/**.h",
			"src/Lucky/Platforms/OpenGLES3/**.cpp",
		}
		-- files
		-- {
		-- 	"src/Lucky/Platforms/Windows/**.h",
		-- 	"src/Lucky/Platforms/Windows/**.cpp",
		-- 	"src/Lucky/Platforms/OpenGL/**.h",
		-- 	"src/Lucky/Platforms/OpenGL/**.cpp",
		-- }


