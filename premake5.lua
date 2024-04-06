include "Dependencies.lua"

workspace "Lucky"  
	startproject "Lucky-Editor"
	configurations 
	{
		"Debug", 
		"Release" 
	} 

	flags 
	{ 
		"MultiProcessorCompile" 
	}

outputdir = "%{cfg.system}\\%{cfg.architecture}\\%{cfg.buildcfg}"
outputdirweb = "browser\\%{cfg.buildcfg}"
tmpdir = "%{cfg.system}\\%{cfg.architecture}"
tmpdirweb = "browser"

if _ACTION == "gmake2" then
	outputdir = "browser\\%{cfg.buildcfg}"
	tmpdir = "browser"
	libDir["GLFW"] = "../Vendors/GLFW/lib-static-ucrt"
end

group "Dependencies"
	if _ACTION == "vs2022" then
		include "Vendors/GLAD/premake5.lua"
	end
	if _ACTION == "gmake2" then
		include "Vendors/Mono/net8.0/lib/src/premake5.lua"
	end
	include "Vendors/ImGui/premake5.lua"
	include "Vendors/ImGuizmo/premake5.lua"
	include "Vendors/yaml-cpp/premake5.lua"
	include "Vendors/Box2D/premake5.lua"

group "Core"
	include "Lucky/premake5.lua"
	if _ACTION == "vs2022" then
		include "Lucky-ScriptCore/premake5.lua"
	end

group "Tools"
	include "LuckyEditor/premake5.lua"

group "Misc"
	include "Sandbox/premake5.lua"

group "Browser/Core"
	include "Browser/BlazorLuckyEditor/premake5.lua"
