_G.package.path=_G.package.path..[[;./?.lua;./?/?.lua]]

assert( require 'premake.quickstart' )

make_solution 'picojson_serializer'

local OS = os.get()

local settings = {
	includedirs = {
		linux = {},
		windows = { './msinttypes' },
		macosx = {}
	}
}

make_console_app('test_ioc_cpp',
	{
		'./*.h',
		'./test/*.cpp',
		'./test/*.h'
	}
)

includedirs {
	'.',
	'./Catch/single_include',
	'./picojson',
	settings.includedirs[OS]
}

run_target_after_build()
