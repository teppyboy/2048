project(
  'sdl2-template',
  'cpp',
  version: '0.1',
  default_options: ['warning_level=3', 'cpp_std=c++20', 'default_library=static'],
)

# Put SDL2 main first to fix build issue on Windows
sdl2_main_dep = dependency('sdl2main')
sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('sdl2_image')
sdl2_ttf_dep = dependency('sdl2_ttf')
sdl2_mixer_dep = dependency('sdl2_mixer')

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language : 'cpp')
endif

exe = executable(
  'sdl2-template',
  'src/main.cpp',
  install: true,
  # win_subsystem: 'windows',
  dependencies: [sdl2_main_dep, sdl2_dep, sdl2_image_dep, sdl2_ttf_dep, sdl2_mixer_dep],
)

test('basic', exe)