local _files = 
{ 
	"**.cs",
	"Properties/**.cs"
}

local _excludes  =
{
	"bin/**/*.*",
	"obj/**/*.*",
}

group "Scripting"
	project "Lucky-ScriptCore"
		kind "SharedLib"
		language "C#"
		dotnetframework "4.8"
		links { "LuckyScripting" }

		targetdir ("%{wks.location}/LuckyEditor/resources/Scripts/Desktop")

		files(_files)
		excludes(_excludes)
		objdir("obj/Desktop")

		filter "configurations:Debug"
			optimize "Off"
			symbols "Default"

		filter "configurations:Release"
			optimize "On"
			symbols "Default"

group "Browser/Scripting"
	project "Lucky-ScriptCore.Web"
		kind "SharedLib"
		language "C#"
		dotnetframework "4.8"
		links { "LuckyScripting.Web" }

		targetname("Lucky-ScriptCore")
		targetdir ("%{wks.location}/LuckyEditor/resources/Scripts/Browser")

		files(_files)
		excludes(_excludes)
		objdir("obj/Browser")

		filter "configurations:Debug"
			optimize "Off"
			symbols "Default"

		filter "configurations:Release"
			optimize "On"
			symbols "Default"
