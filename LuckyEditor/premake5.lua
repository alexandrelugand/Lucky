local _files =
{ 
	"**.h", 
	"**.cpp", 
	"%{includeDir.GLM}/glm/**.hpp", 
	"%{includeDir.GLM}/glm/**.inl" 
}

local _vpaths =
{
	["Vendors/*"] = 
	{ 
		"%{includeDir.GLM}/glm/**.hpp", 
		"%{includeDir.GLM}/glm/**.inl"
	}
}

local _includedirs =
{
	"src",
	"../Lucky/src",
	"%{includeDir.spdlog}",
	"%{includeDir.GLFW}",
	"%{includeDir.GLAD}",
	"%{includeDir.ImGui}",
	"%{includeDir.GLM}",
	"%{includeDir.entt}",
	"%{includeDir.ImGuizmo}",
	"%{includeDir.nameof}"
}

local _libdirs =
{
	"%{libDir.GLFW}",
	"%{libDir.ImGui}"
}

group "Core"
	project "LuckyEditor"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++20"
		architecture "x64"
		targetdir ("%{wks.location}/bin/" .. outputdir)
		objdir ("%{wks.location}/bin-int/" .. tmpdir)
		staticruntime "off"

		dependson { "GLAD", "ImGui", "yaml-cpp", "Lucky" }

		files(_files)
		vpaths(_vpaths)
		includedirs(_includedirs)
		libdirs(_libdirs)
		disablewarnings { "4996" }

		excludes {
			"src/Bootstrapper.h",
			"src/Bootstrapper.cpp",
			"src/Bootstrapper_wrap.cpp",
		}

		links
		{
			"Lucky",
			"ImGui",
			"ImGuizmo",
			"yaml-cpp",
			"Box2D",
			"glfw3_mt"
		}

		defines
		{
			"PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}

		-- Precompile header
		pchheader "LuckyEditorPch.h"
		pchsource "src/LuckyEditorPch.cpp"

		-- Configurations
		filter "configurations:Debug"
			defines { "DEBUG" }
			runtime "Debug"
			symbols "On" 
			optimize "Off"
			ignoredefaultlibraries { "LIBCMT" }
			debugargs { "assets\\scenes\\Physics.lucky" }

		filter "configurations:Release"  
			defines { "NDEBUG" }
			runtime "Release"
			optimize "full"

group "Browser/Core"
	project "LuckyEditor.Web"
		kind "StaticLib"
		targetname "LuckyEditor"
		targetextension ".a"
		language "C++"
		cppdialect "C++20"
		architecture "x64"
		targetdir ("%{wks.location}/bin/" .. outputdirweb)
		objdir ("%{wks.location}/bin-int/" .. tmpdirweb)
		staticruntime "off"

		dependson { "ImGui.Web", "yaml-cpp.Web", "Lucky.Web" }

		files(_files)
		vpaths(_vpaths)
		includedirs(_includedirs)
		libdirs(_libdirs)

		links
		{
			"Lucky.Web",
			"ImGui.Web",
			"ImGuizmo.Web",
			"yaml-cpp.Web",
			"Box2D.Web"
		}

		defines
		{
			"__EMSCRIPTEN__"
		}

		-- Precompile header
		pchheader "src/LuckyEditorPch.h"
		pchsource "src/LuckyEditorPch.cpp"
		pchoutputfile "LuckyEditorPch.h.gch"

		-- Prebuild
		prebuildmessage "Running swig language bridge..."
		prebuildcommands { 
			"set \"SWIG_LIB=$(SWIG_LIB_DIRECTORY)\"",
			"\"$(SWIG_EXECUTABLE)\" -c++ -csharp -outdir \"$(SolutionDir)Browser\\BlazorLuckyEditor\\SWIG\" -o \"$(ProjectDir)src\\Bootstrapper_wrap.cpp\" \"$(ProjectDir)src\\Bootstrapper.i\"",
			"\"$(PYTHON)\" \"$(SolutionDir)Scripts\\fixswig.py\" \"$(SolutionDir)Browser\\BlazorLuckyEditor\\SWIG\\LuckyEditorPINVOKE.cs\""
		}

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
