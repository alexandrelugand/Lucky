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
tmpdir = "%{cfg.system}\\%{cfg.architecture}"

includeDir = {}
includeDir["spdlog"]="../Vendors/spdlog/include"
includeDir["GLFW"]="../Vendors/GLFW/include"
includeDir["GLAD"]="../Vendors/GLAD/include"
includeDir["ImGui"]="../Vendors/ImGui"
includeDir["GLM"]="../Vendors/GLM"
includeDir["stb"]="../Vendors/stb"
includeDir["entt"]="../Vendors/entt/include"

libDir = {}
libDir["GLFW"]= iif(_ACTION == "gmake2", "../Vendors/GLFW/lib-static-ucrt", "../Vendors/GLFW/lib-vc2022")


group "Dependencies"
	include "Vendors/GLAD/premake5.lua"
	include "Vendors/ImGui/premake5.lua"

group ""
	include "Lucky/premake5.lua"
	include "LuckyEditor/premake5.lua"
	include "Sandbox/premake5.lua"
