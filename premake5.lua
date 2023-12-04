workspace "SDL"  
    configurations { "Debug", "Release" } 

outputdir = "%{cfg.buildcfg}\\%{cfg.system}\\%{cfg.architecture}"

include "Lucky/premake5.lua"
include "App/premake5.lua"
