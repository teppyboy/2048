project(
  '2048',
  'cpp',
  version: '1.1.4',
  default_options: ['warning_level=3', 'cpp_std=c++20', 'default_library=shared'],
)

meson.add_install_script('scripts/copy_deps.py')

# Put SDL2 main first to fix build issue on Windows
sdl2_main_dep = dependency('sdl2main')
sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('sdl2_image')
sdl2_ttf_dep = dependency('sdl2_ttf')
sdl2_mixer_dep = dependency('sdl2_mixer')
json = dependency('nlohmann_json')

win_subsystem = 'console'

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language: 'cpp')
else
  win_subsystem = 'windows'
endif

exe = executable(
  '2048',
  'src/main.cpp',
  install: true,
  install_dir: meson.current_source_dir() + '/bin',
  win_subsystem: win_subsystem,
  dependencies: [sdl2_main_dep, sdl2_dep, sdl2_image_dep, sdl2_ttf_dep, sdl2_mixer_dep, json],
)

test('basic', exe)