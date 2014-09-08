_G.package.path=_G.package.path..[[;./?.lua;./?/?.lua]]

assert( require 'premake.quickstart' )

make_solution 'picojson_serializer'

configurations { "gcov" }

local CURRENT_VERSION = 'v0.9.0'

local OS = os.get()

local settings = {
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
configuration { "*" }

includedirs {
	'.',
	'./Catch/single_include',
	'./picojson',
	settings.includedirs[OS]
}

run_target_after_build()

make_package(   {
            		'*.h',
            		'picojson/*',
            		'README.md'
                },
                'picojson_serializer-' .. CURRENT_VERSION
            )
