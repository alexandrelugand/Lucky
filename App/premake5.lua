project "App"  
    kind "ConsoleApp"   
    language "C++"   
    cppdialect "C++17"
    architecture "x64"
    targetdir ("%{wks.location}/bin/" .. outputdir)
    objdir ("%{wks.location}/bin-int/" .. outputdir)

    files { "**.h", "**.cpp" } 

    includedirs
    {
        "src",
        "../Lucky/src",
        "../Vendors/spdlog/include",
        "../Vendors/GLFW/include"
    }

    libdirs 
    {
         "../Vendors/GLFW/lib-static-ucrt"
    }

    links
    {
        "Lucky",
        "opengl32",
        "GLFW3"
    }

    filter "configurations:Debug"
        defines { "DEBUG", "SDL_MAIN_HANDLED" }  
        symbols "On" 

    filter "configurations:Release"  
        defines { "NDEBUG", "SDL_MAIN_HANDLED" }    
        optimize "On" 

    filter "system:windows"
		systemversion "latest"

		defines { "PLATFORM_WINDOWS" }

		postbuildcommands 
        {
			'{COPY} "../Vendors/GLFW/lib-static-ucrt/glfw3.dll" "%{cfg.targetdir}"',
		}