group "Scripting"

	externalproject "LuckyScripting"
		location "."
		kind "SharedLib"
		language "C#"
		dependson { "Lucky" }

group "Browser/Scripting"

	externalproject "LuckyScripting.Web"
		location "."
		kind "SharedLib"
		language "C#"
		dependson { "Lucky.Web" }
