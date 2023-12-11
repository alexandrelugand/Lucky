workspace "Lucky"  
	configurations { "Debug", "Release" } 

outputdir = "%{cfg.system}\\%{cfg.architecture}\\%{cfg.buildcfg}"
tmpdir = "%{cfg.system}\\%{cfg.architecture}"

includeDir = {}
includeDir["spdlog"]="../Vendors/spdlog/include"
includeDir["GLFW"]="../Vendors/GLFW/include"
includeDir["GLAD"]="../Vendors/GLAD/include"
includeDir["ImGui"]="../Vendors/ImGui"
includeDir["GLM"]="../Vendors/GLM"

libDir = {}
libDir["GLFW"]="../Vendors/GLFW/lib-static-ucrt"

include "Vendors/GLAD/premake5.lua"
include "Vendors/ImGui/premake5.lua"
include "Lucky/premake5.lua"
include "App/premake5.lua"
