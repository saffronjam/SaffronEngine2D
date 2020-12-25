-- -------------------------------------------------
-- -------------------------------------------------
-- --------- Saffron 2D Project Generator ----------
-- -------------------------------------------------
-- -------------------------------------------------


-- --------------------------------------
-- Saffron Workspace
-- --------------------------------------

workspace "Saffron"
	architecture "x64"
	
	configurations 
	{ 
		"Debug", 
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	startproject "Project"
	


-- VARIABLES
SfmlLibs = "../Deps/SFML/lib"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Saffron"] = "Saffron/Src"
IncludeDir["Box2D"] = "Deps/Box2D/include"
IncludeDir["entt"] = "Deps/entt/include"
IncludeDir["FastNoise"] = "Deps/FastNoise/include"
IncludeDir["ImGui"] = "Deps/ImGui"
IncludeDir["SFML"] = "Deps/SFML/include"
IncludeDir["spdlog"] = "Deps/spdlog/include"
IncludeDir["yamlcpp"] = "Deps/yaml-cpp/include"

LibraryDir = {}

SharedLibraryDir = {}

RuntimeLibararyDir = {}

outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- --------------------------------------
-- Engine
-- --------------------------------------

group "Engine"

group "Engine/Dependencies"
	include "Deps/Box2D/premake5"
	include "Deps/entt/premake5"
	include "Deps/FastNoise/premake5"
	include "Deps/ImGui/premake5"
	include "Deps/SFML/premake5"
	include "Deps/spdlog/premake5"
	include "Deps/yaml-cpp/premake5"	
group "Engine"

-- --------------------------------------
-- Saffron
-- --------------------------------------

project "Saffron"
	location "Saffron"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("Bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("Bin-Int/" .. outputDirectory .. "/%{prj.name}")

	pchheader "SaffronPCH.h"
	pchsource "Saffron/Src/SaffronPCH.cpp"

	files 
	{ 
		"%{prj.name}/Src/**.h", 
		"%{prj.name}/Src/**.c", 
		"%{prj.name}/Src/**.hpp", 
		"%{prj.name}/Src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Src",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.FastNoise}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.SFML}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.yamlcpp}"
	}
	
	defines
	{
		"SFML_STATIC"
	}
	
	links 
	{
		"SFML",
		"Box2D",
		"FastNoise",
		"ImGui",
		"opengl32.lib",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines 
		{ 
			"_CRT_SECURE_NO_WARNINGS",			
			"SE_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "SE_DEBUG"
		symbols "On"
			
	filter "configurations:Release"
		defines "SE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SE_DIST"
		optimize "On"
		
group ""


-- --------------------------------------
-- Project
-- --------------------------------------
		
project "Project"
	location "Project"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("Bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("Bin-Int/" .. outputDirectory .. "/%{prj.name}")
	
	files 
	{ 
		"%{prj.name}/Src/**.h", 
		"%{prj.name}/Src/**.c", 
		"%{prj.name}/Src/**.hpp", 
		"%{prj.name}/Src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Src",
		"Saffron/Src",
		"%{IncludeDir.Saffron}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.FastNoise}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.SFML}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.yamlcpp}"
	}
	
	defines
	{
		"SFML_STATIC"
	}

	links 
	{			
		"SFML",
		"Saffron",
		"Box2D",
		"FastNoise",
		"ImGui",
		"yaml-cpp"
	}
	
	postbuildcommands 
	{
		'{COPY} "../Resources/" "%{cfg.targetdir}/Resources/"',
		'{COPY} "../Resources/" "Resources/"'
	}
	
	filter "system:windows"
		systemversion "latest"
		
		defines 
		{ 
			"_CRT_SECURE_NO_WARNINGS",			
			"SE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SE_DEBUG"
		symbols "On"
			
	filter "configurations:Release"
		defines "SE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SE_DIST"
		optimize "On"
		