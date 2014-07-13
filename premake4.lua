local OS=os.get()

local definitions = {
	dir = {
		linux = "ls",
		windows = "dir",
		macosx = {}
	},
	includedirs = {
		linux = {},
		windows = { './msinttypes' },
		macosx = {}
	}
}

local cfg={}

for i,v in pairs(definitions) do
 cfg[i]=definitions[i][OS]
end

-- Apply to current "filter" (solution/project)
function DefaultConfig()
	location "Build"
	configuration "Debug"
		defines { "DEBUG", "_DEBUG" }
		objdir "Build/obj"
		targetdir "Build/Debug"
		flags { "Symbols" }
	configuration "Release"
		defines { "RELEASE" }
		objdir "Build/obj"
		targetdir "Build/Release"
		flags { "Optimize" }
	configuration "*" -- to reset configuration filter
end

function CompilerSpecificConfiguration()
	configuration {"xcode*" }
		--postbuildcommands {"$TARGET_BUILD_DIR/$TARGET_NAME"}

	configuration {"gmake"}
		postbuildcommands  { "$(TARGET)" }

	configuration {"codeblocks" }
		postbuildcommands { "$(TARGET_OUTPUT_FILE)"}

	configuration { "vs*"}
		postbuildcommands { "\"$(TargetPath)\"" }
	configuration { "*" }
end

-- A solution contains projects, and defines the available configurations
local sln=solution "picojson_serializer"
    location "Build"
	sln.absbasedir=path.getabsolute(sln.basedir)
	configurations { "Debug", "Release" }
	includedirs( cfg.includedirs )
	platforms { "native" , "x32", "x64" }
	includedirs {
		sln.basedir,
		path.join(sln.basedir,"Catch/single_include"),
		path.join(sln.basedir,"picojson"),
		path.join(sln.basedir,"test")
	}
	vpaths {
		["Headers"] = "**.h",
		["Sources"] = {"**.cc", "**.cpp"},
	}

----------------------------------------------------------------------------------------------------------------

local tests=project "test"
	local basedir="test"
	kind "ConsoleApp"
	DefaultConfig()
	language "C++"
	files {
		path.join(basedir,"**.cpp"),
		path.join(basedir,"**.h"),
		path.join(basedir,"../*.h")
	}
	CompilerSpecificConfiguration()
