---@diagnostic disable: undefined-global, undefined-field

Inspect = require('inspect')
Utils = require('utils')

OutBin     = _MAIN_SCRIPT_DIR .. "/Build/Bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}/"
OutObj     = _MAIN_SCRIPT_DIR .. "/Build/Obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}/"
OutBinDist = _MAIN_SCRIPT_DIR .. "/Example/%{cfg.system}/"
OutLoc     = _MAIN_SCRIPT_DIR .. "/Build/"
PrjLoc     = _MAIN_SCRIPT_DIR .. "/Build/"
WrkLoc     = _MAIN_SCRIPT_DIR .. "/"
AstFol     = "Assets/"

local function GetBasePath()
	return debug.getinfo(1).source:match("@?(.*/)")
end

local function RequireAll()
    local result = {}
    result["ImGui"]  = require("ThirdParty.imgui.premake5")
    result["SFML"]   = require("ThirdParty.SFML.premake5")
    result["Glad"]   = require("ThirdParty.Glad.premake5")
    result["Jcv"]    = require("ThirdParty.jcv.premake5")
    result["Spdlog"] = require("ThirdParty.spdlog.premake5")
    result["Box2D"]  = require("ThirdParty.Box2D.premake5")
    return result
end

local ThirdParties

local function LinkAll()
    for k, v in pairs(ThirdParties) do
        if v.Link ~= nil then
            v.Link()            
        end
    end
end

local function IncludeAll()
    for k, v in pairs(ThirdParties) do
        if v.Include ~= nil then
            v.Include()            
        end
    end
end

--[[
 Called by Client, because only Client knows
 the BinaryOutputDir for sure
]]
local function PreBuildAll(Configuration, BinaryOutputDir, ProjectDir)
    for k, v in pairs(ThirdParties) do
        if v.PreBuild ~= nil then
            v.PreBuild(Configuration, BinaryOutputDir, ProjectDir)            
        end
    end
end

--[[
 Called by Client, because only Client knows
 the BinaryOutputDir for sure
]]
local function PostBuildAll(Configuration, BinaryOutputDir, ProjectDir)
    for k, v in pairs(ThirdParties) do
        if v.PostBuild ~= nil then
            v.PostBuild(Configuration, BinaryOutputDir, ProjectDir)            
        end
    end
end


-- Start module

local module = {}

module.Project = "SaffronEngine2D"

module.Include = function ()
    includedirs {
        GetBasePath() .. "Source"
    }    
    -- Include third parties
    IncludeAll()
end

module.Link = function ()
    links {
        module.Project
    }
end

module.PreBuild = function (Configuration, BinaryOutputDir, ProjectDir)
    disablewarnings { 
        "4244",
        "4267"
    }
    PreBuildAll(Configuration, BinaryOutputDir, ProjectDir)
end

module.PostBuild = function (Configuration, BinaryOutputDir, ProjectDir)    
	filter ("configurations:" .. Configuration)
	    local resFrom = GetBasePath() .. AstFol
        local resBinTo = BinaryOutputDir .. AstFol
        local resProjTo = ProjectDir .. AstFol
        postbuildcommands {
            Utils.CopyCmd(resFrom, resBinTo),
            Utils.CopyCmd(resFrom, resProjTo),
        }
    PostBuildAll(Configuration, BinaryOutputDir, ProjectDir)
end

module.SetStartUpProject = function (projectName)
    workspace "Saffron"
        startproject (projectName)
end


group "Engine/ThirdParty"
ThirdParties = RequireAll()
group "Engine"
module.PostBuildSfml = ThirdParties["SFML"].PostBuild

module.AddDefines = function()
	filter "configurations:Debug or Release or Dist"
        defines {
            "SE_IMGUI_INI_PATH=\"Assets/imgui.ini\""
        }
    filter "system:windows"
        defines {
            "_CRT_SECURE_NO_WARNINGS",
            "SE_PLATFORM_WINDOWS",
        }
    filter "configurations:Debug"
        defines { "DEBUG", "SE_DEBUG" }
    filter "configurations:Release"
        defines { "NDEBUG", "SE_RELEASE" }
    filter "configurations:Dist"
        defines { "NDEBUG", "SE_DIST" }
end

project (module.Project)
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
	staticruntime "On"

	pchheader ("SaffronPCH.h")
	pchsource ("Source/SaffronPCH.cpp")

	targetdir (OutBin)
	objdir (OutObj)
	location (OutLoc)

    files {
        "Source/**.h",
		"Source/**.c",
		"Source/**.hpp",
		"Source/**.cpp",
    }
    
    disablewarnings { 
        "4244",
        "4267"
    }

    -- Include self and third parties
    module.Include()

    -- Link third parties
    LinkAll()
    
    module.AddDefines()

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"
    filter "configurations:Dist"
        optimize "On"

group ""

return module