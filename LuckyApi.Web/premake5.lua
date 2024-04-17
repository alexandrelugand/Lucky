group "Browser/Scripting"

externalproject "LuckyApi.Web"
	location "."
	kind "SharedLib"
	language "C#"
	dependson { "Lucky.Web" }
