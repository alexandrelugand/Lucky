project "Lucky"  
    kind "StaticLib"   
    language "C++"   
    cppdialect "C++17"
    architecture "x64"
    targetdir ("%{wks.location}/bin/" .. outputdir)
    objdir ("%{wks.location}/bin-int/" .. outputdir)

    files { "**.h", "**.cpp" } 

    includedirs
    {
        "src",
        "../Vendors/spdlog/include",
        "../Vendors/GLFW/include"
    }

    libdirs 
    {
         "../Vendors/GLFW/lib-static-ucrt"
    }

    links
    {
        "opengl32",
        "GLFW3"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }  
        symbols "On" 

    filter "configurations:Release"  
        defines { "NDEBUG" }    
        optimize "On" 
