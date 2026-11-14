project('curp', 'c',
  version : '0.1',
  license: 'LGPL-2.1-only',
  default_options : ['warning_level=3'])

inc = include_directories('include')

# include
install_headers('include/curp.h')

# src
curp_sources = ['src/curp.c', 'src/trie.c']
curplib = shared_library('curp',
                         curp_sources,
                         include_directories : inc,
                         install : true)

# tests
check_dep = dependency('check', required : true)
theft_dep = dependency('libtheft', required : true)

test_sources = ['tests/check_curp.c',
                'tests/check_curp_examples.c',
                'tests/check_curp_pbt.c']
testexe = executable('testexe', test_sources,
                     link_with : curplib,
                     include_directories : inc,
                     dependencies : [check_dep, theft_dep])

test('curp test', testexe)

# pkg
pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : curplib,
                 version : '0.1',
                 name : 'libcurp',
                 filebase : 'curp',
                 description : 'Valida la CURP Mexicana.')
