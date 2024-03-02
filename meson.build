project('reach', 'c', 'cpp')

# shader

shdc = find_program('sokol-shdc',
    dirs: [
        join_paths(meson.current_source_dir(), 'tools/sokol-tools-bin/bin/win32'),
        join_paths(meson.current_source_dir(), 'tools/sokol-tools-bin/bin/linux')],
    required: true)

shader = custom_target('targetname',
    output : 'unlit.glsl.h',
    input : 'src/shader/unlit.glsl',
    command : [shdc, '--slang=glsl330', '--input=@INPUT@', '--output=@OUTPUT@'])

# main executable

cmake = import('cmake')

deps = [
    cmake.subproject('expected').dependency('expected'),
    cmake.subproject('spdlog').dependency('spdlog'),
    cmake.subproject('glm').dependency('glm'),
    cmake.subproject('glfw').dependency('glfw'),
    cmake.subproject('entt').dependency('EnTT'),
    subproject('sokol').get_variable('sokol_dep')
]

if build_machine.system() == 'linux'
    deps += dependency('gl')
endif

src = [
    'src/main.cpp',
    'src/impl.c',
    'src/app.cpp',
    'src/world.cpp',
    'src/manager/window_manager.cpp',
    'src/manager/device_manager.cpp',
    'src/manager/material_manager.cpp',
    'src/systems/transform_system.cpp',
    'src/systems/render_system.cpp'
]

executable('reach',
    src + shader,
    dependencies: deps,
    include_directories: ['src/'],
    override_options : ['cpp_std=c++20', 'cpp_eh=none', 'cpp_rtti=false', 'werror=true', 'warning_level=3'],
    cpp_args : ['-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING', '-D_HAS_EXCEPTIONS=0', '-DGLM_ENABLE_EXPERIMENTAL', '-DSPDLOG_NO_EXCEPTIONS', '-DENTT_NOEXCEPTION'])
