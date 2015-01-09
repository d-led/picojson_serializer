include 'premake'

make_solution 'picojson_serializer'

platforms { 'native' }

configurations { 'gcov' }

local CURRENT_VERSION = 'v0.9.0'

local OS = os.get()

local local_settings = {
	includedirs = {
		linux = {},
		windows = { './msinttypes' },
		macosx = {}
	}
}

make_console_app('picojson_serializer_test',
	{
		'./*.h',
		'./test/*.cpp',
		'./test/*.h'
	}
)

configuration { "linux", "gcov" }
	flags { "Symbols" }
	links { "gcov" }
	buildoptions { "-coverage" }
	print 'hoho'
configuration { "*" }

includedirs {
	'.',
	'./Catch/single_include',
	'./picojson',
	local_settings.includedirs[OS]
}

run_target_after_build()

local release = dofile 'premake/release.lua'

release.make_package(   {
            		'*.h',
            		'picojson/*',
            		'README.md'
                },
                'picojson_serializer-' .. CURRENT_VERSION
            )
