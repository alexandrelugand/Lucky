group "Core"
	project "Lucky-ScriptCore"
		kind "SharedLib"
		language "C#"
		dotnetframework "4.8"

		targetdir ("%{wks.location}/LuckyEditor/resources/Scripts")

		files 
		{
			"Source/**.cs",
			"Properties/**.cs"
		}

		filter "configurations:Debug"
			optimize "Off"
			symbols "Default"

		filter "configurations:Release"
			optimize "On"
			symbols "Default"
