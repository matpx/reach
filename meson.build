project('reach', 'c', 'cpp', default_options: ['cpp_std=c++20'])

cmake = import('cmake')

expected_lib = cmake.subproject('expected').dependency('expected')
glfw_lib = cmake.subproject('glfw').dependency('glfw')
entt_lib = cmake.subproject('entt').dependency('EnTT')

src = [
    'src/main.cpp',
    'src/impl.c',
    'src/app.cpp',
    'src/window.cpp',
    'src/renderer.cpp'
]

shdc = find_program('sokol-shdc', dirs: [join_paths(meson.current_source_dir(), 'tools/sokol-tools-bin/bin/win32')], required: true)

shader = custom_target('targetname',
    output : 'unlit.glsl.h',
    input : 'src/shader/unlit.glsl',
    command : [shdc, '--slang=glsl330', '--input=@INPUT@', '--output=@OUTPUT@'])

executable('reach',
    src + shader,
    include_directories: ['subprojects'],
    dependencies: [expected_lib, glfw_lib, entt_lib])
