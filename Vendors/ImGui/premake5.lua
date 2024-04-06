local _files =
{
	"imconfig.h",
	"imgui.h",
	"imgui.cpp",
	"imgui_draw.cpp",
	"imgui_internal.h",
	"imgui_tables.cpp",
	"imgui_widgets.cpp",
	"imstb_rectpack.h",
	"imstb_textedit.h",
	"imstb_truetype.h",
	"imgui_demo.cpp",
	"backends/imgui_impl_glfw.cpp",
	"backends/imgui_impl_glfw.cpp",
	"backends/imgui_impl_opengl3.h",
	"backends/imgui_impl_opengl3.cpp"
}

local _includedirs =
{
	".",
	"./backends",
	"../GLFW/include"
}

group "Dependencies"
	project "ImGui"
		kind "StaticLib"
		language "C++"
		cppdialect "C++20"
		staticruntime "off"
		architecture "x64"
		targetdir ("%{wks.location}/bin/" .. outputdir)
		objdir ("%{wks.location}/bin-int/" .. tmpdir)

		files(_files)
		includedirs(_includedirs)

		filter "system:windows"
			systemversion "latest"

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
			optimize "Off"

		filter "configurations:Release"
			runtime "Release"
			optimize "full"

group "Browser/Dependencies"
	project "ImGui.Web"
		kind "StaticLib"
		targetname "ImGui"
		targetextension ".a"
		language "C++"
		cppdialect "C++20"
		staticruntime "off"
		architecture "x64"
		targetdir ("%{wks.location}/bin/" .. outputdirweb)
		objdir ("%{wks.location}/bin-int/" .. tmpdirweb)

		files(_files)
		includedirs(_includedirs)

		filter "system:windows"
			systemversion "latest"

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
			optimize "Off"

		filter "configurations:Release"
			runtime "Release"
			optimize "full"

group ""