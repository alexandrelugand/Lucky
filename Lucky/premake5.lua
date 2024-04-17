local _files = 
{ 
	"**.h",
	"**.cpp",
	"src/Lucky/LuckyApi_wrap.cpp",
	"%{includeDir.GLM}/glm/**.hpp", 
	"%{includeDir.GLM}/glm/**.inl",
	"%{includeDir.stb}/**.h", 
	"%{includeDir.stb}/**.cpp",
}

local _vpaths =
{ 
	["Vendors/*"] = 
	{ 
		"%{includeDir.GLM}/glm/**.hpp", 
		"%{includeDir.GLM}/glm/**.inl",
		"%{includeDir.stb}/**.h",
		"%{includeDir.stb}/**.cpp"
	} 
}

local _includedirs =
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
	"%{includeDir.nameof}",
	"%{includeDir.Box2D}",
	"%{includeDir.VulkanSDK}"
}

local _libdirs =
{
	"%{libDir.GLFW}"
}

group "Core"
	project "Lucky"
		kind "StaticLib"
		language "C++"
		cppdialect "C++20"
		architecture "x64"
		targetdir ("%{wks.location}/bin/" .. outputdir)
		objdir ("%{wks.location}/bin-int/" .. tmpdir)
		staticruntime "off"

		dependson { "GLAD", "ImGui", "yaml-cpp" }

		files(_files)
		vpaths(_vpaths)
		includedirs(_includedirs)
		libdirs(_libdirs)
		disablewarnings { "4996" }

		links
		{
			"opengl32",
			"GLAD",
			"ImGui",
			"ImGuizmo",
			"yaml-cpp",
			"Box2D"		
		}

		defines
		{
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE",
			"PLATFORM_WINDOWS", 
			"_CRT_SECURE_NO_WARNINGS", 
			"OPENGL"
		}

		excludes
		{
			"src/Lucky/Platforms/Browser/**.h",
			"src/Lucky/Platforms/Browser/**.cpp"
		}

		-- Precompile header
		pchheader "LuckyPch.h"
		pchsource "src/LuckyPch.cpp"

		-- Prebuild
		prebuildmessage "Running swig language bridge..."
		prebuildcommands { 
			"set \"SWIG_LIB=$(SWIG_LIB_DIRECTORY)\"",
			"\"$(SWIG_EXECUTABLE)\" -c++ -csharp -namespace Lucky -outdir \"$(SolutionDir)LuckyApi\\SWIG\" -o \"$(ProjectDir)src\\Lucky\\LuckyApi_wrap.cpp\" \"$(ProjectDir)src\\Lucky\\LuckyApi.i\"",
			"\"$(PYTHON)\" \"$(SolutionDir)Scripts\\fixswig.py\" \"$(SolutionDir)LuckyApi\\SWIG\\LuckyPINVOKE.cs\" \"Lucky\""
		}

		filter "files:../Vendors/ImGuizmo/*.cpp"
			flags { "NoPCH" }

		filter "files:src/Lucky/LuckyApi*.cpp"
			flags { "NoPCH" }

		-- Configurations
		filter "configurations:Debug"
			defines { "DEBUG" }
			runtime "Debug"
			symbols "On" 
			optimize "Off"

			links 
			{
				"%{library.ShaderC_Debug}",
				"%{library.SPIRV_Cross_Debug}",
				"%{library.SPIRV_Cross_GLSL_Debug}"
			}

		filter "configurations:Release"
			defines { "NDEBUG" }
			runtime "Release"
			optimize "full"

			links 
			{
				"%{library.ShaderC_Release}",
				"%{library.SPIRV_Cross_Release}",
				"%{library.SPIRV_Cross_GLSL_Release}"
			}

		filter "action:vs2022"
			includedirs
			{
				"%{includeDir.mono}"
			}

			libdirs 
			{
				"%{libDir.mono}"
			}

			links 
			{ 
				"glfw3_mt",
				"%{library.mono}",
				"%{library.WinSock}",
				"%{library.Winmm}",
				"%{library.WinVersion}",
				"%{library.BCrypt}"
			}

group "Browser/Core"
	project "Lucky.Web"
		kind "StaticLib"
		targetname "Lucky"
		targetextension ".a"
		language "C++"
		cppdialect "C++20"
		architecture "x64"
		targetdir ("%{wks.location}/bin/" .. outputdirweb)
		objdir ("%{wks.location}/bin-int/" .. tmpdirweb)
		staticruntime "off"

		dependson { "ImGui.Web", "yaml-cpp.Web" }

		files(_files)
		vpaths(_vpaths)
		includedirs(_includedirs)

		libdirs 
		{
			"%{libDir.GLFW}"
		}

		links
		{
			-- "opengl32",
			"ImGui.Web",
			"ImGuizmo.Web",
			"yaml-cpp.Web",
			"Box2D.Web"
		}

		defines
		{
			"__EMSCRIPTEN__",
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE"
		}

		excludes
		{
			"src/Lucky/Platforms/Windows/**.h",
			"src/Lucky/Platforms/Windows/**.cpp",
			"src/Lucky/Platforms/OpenGL/**.h",
			"src/Lucky/Platforms/OpenGL/**.cpp",
		}

		-- Precompile header
		pchheader "src/LuckyPch.h"
		pchsource "src/LuckyPch.cpp"
		pchoutputfile "LuckyPch.h.gch"

			-- Prebuild
		prebuildmessage "Running swig language bridge..."
		prebuildcommands { 
			"set \"SWIG_LIB=$(SWIG_LIB_DIRECTORY)\"",
			"\"$(SWIG_EXECUTABLE)\" -c++ -csharp -namespace Lucky -outdir \"$(SolutionDir)LuckyApi.Web\\SWIG\" -o \"$(ProjectDir)src\\Lucky\\LuckyApi_wrap.cpp\" \"$(ProjectDir)src\\Lucky\\LuckyApi.i\"",
			"\"$(PYTHON)\" \"$(SolutionDir)Scripts\\fixswig.py\" \"$(SolutionDir)LuckyApi.Web\\SWIG\\LuckyPINVOKE.cs\""
		}

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
