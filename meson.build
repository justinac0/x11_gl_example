project(
  'x11_gl_example',
  'c',
  version: '0.0.0',
  default_options: ['c_std=c99'],
)

host = host_machine.system()

supported_hosts = ['windows', 'linux']

if host not in supported_hosts
  error('host system \'' + host + '\' not supported')
endif

window_option = get_option('window_system')
if window_option == 'xlib'
  ws_dep = dependency('x11', required: window_option == 'xlib')
  add_project_arguments('-DWINDOW_XLIB', language: 'c')
elif window_option == 'xcb'
  ws_dep = dependency('xcb', required: window_option == 'xcb')
  add_project_arguments('-DWINDOW_XCB', language: 'c')
else
endif

cc = meson.get_compiler('c')

cflags = [
  '-Wall',
  '-Wextra',
  # '-Werror',
  # '-Wshadow',
  '-Wpedantic',
  '-Wno-unused-function',
  # '-Wno-gnu-zero-variadic-macro-arguments',
  '-Wno-unused-function',
  '-Wno-undefined-internal',
  # '-fsanitize=address',
]

add_project_arguments(cflags, language: 'c')

gl_dep = dependency('gl', required: true)
m_dep = cc.find_library('m', required: true)

deps = [ws_dep, gl_dep, m_dep]

incdir = include_directories(['thirdparty/glad/include', 'thirdparty/', './src/'])
src = 'src/build.c'

executable('app', sources: src, include_directories: incdir, dependencies: deps)
