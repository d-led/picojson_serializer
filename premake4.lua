_G.package.path=_G.package.path..[[;./?.lua;./?/?.lua]]

assert( require 'premake.quickstart' )

make_solution 'picojson_serializer'

local CURRENT_VERSION = 'v0.9.0'

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

newaction {
        trigger     = 'package',
        description = 'prepare a binary release',
        execute     = function ()
        	local to_package = {
        		'*.h',
        		'picojson/*',
        		'README.md'
        	}
        	local command = 'tar -cvpzf picojson_serializer-' ..
        					CURRENT_VERSION ..
        					'.tar.gz ' ..
        					table.concat( to_package, ' ')
        	print(command)
            os.execute(command)
        end
}