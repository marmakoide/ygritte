#! /usr/bin/env python

# the following two variables are used by the target "waf dist"
VERSION = '1.0.0'
APPNAME = 'ygritte'

# these variables are mandatory ('/' are converted automatically)
top, out = '.', 'build'



def options(context):
	context.load('compiler_cxx')



def configure(context):
	context.setenv('debug')
	context.load('compiler_cxx')
	context.env.CXXFLAGS = ['-std=c++14', '-Wall', '-Wextra', '-O3', '-g', '-frounding-math']

	context.setenv('release', env = context.env.derive())



def init(context):
	from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext

	for x in 'debug release'.split():
		for y in (BuildContext, CleanContext, InstallContext, UninstallContext):
			name = y.__name__.replace('Context','').lower()
			class tmp(y):
				cmd = name + '_' + x
				variant = x



def build(context):
	if not context.variant:
		context.fatal('Call "waf build_debug" or "waf build_release", and read the comments in the wscript file!')



	'''
	ygritte library
	'''

	# Headers
	context(
		name            = 'ygritte',
		includes        = 'include',
		export_includes = 'include'
	)

	# The pkg-config file
	context(
		source  = 'ygritte.pc.in',
		VERSION = context.env['VERSION'],
		PREFIX  = context.env['PREFIX'])

	# Specify what to install
	context.install_files(
		'${PREFIX}',
		context.path.ant_glob('include/ygritte/*.h'),
		relative_trick = True
	)



	'''
	unit-testing program
	'''
	if context.variant == 'debug':
		context.program(
			target   = 'ygritte-unitest',
			includes = 'tests',
			source   = context.path.ant_glob('tests/*.cpp'),
			lib      = ['m'],
			use      = ['ygritte']
		)

