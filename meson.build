project('reach', 'c', 'cpp')

cmake = import('cmake')

# shader

shader_make = find_program('ShaderMake', dirs : [join_paths(meson.current_source_dir(), 'tools/ShaderMake')], required : true)

shader_make_target = custom_target(
    'shader_make',
    build_always: true,
    output : ['fake'],
    command : [shader_make, '--compiler', '@CURRENT_SOURCE_DIR@/tools/ShaderMake/fxc.exe', '-c', '@CURRENT_SOURCE_DIR@/src/shader/shader.cfg', '-o', '@BUILD_ROOT@', '--header', '-p', 'DXBC'])

# main executable

deps = [
    cmake.subproject('expected').dependency('expected'),
    cmake.subproject('GSL').dependency('GSL'),
    cmake.subproject('spdlog').dependency('spdlog'),
    cmake.subproject('concurrentqueue').dependency('concurrentqueue'),
    cmake.subproject('glm').dependency('glm'),
    cmake.subproject('entt').dependency('EnTT'),
    subproject('cgltf').get_variable('cgltf_dep'),
    subproject('rjm').get_variable('rjm_dep'),
    subproject('JoltPhysics').get_variable('jolt_dep')
]

# if not meson.is_cross_build()
    deps += cmake.subproject('glfw').dependency('glfw')
# endif

# if build_machine.system() == 'linux'
#     deps += dependency('gl')
# endif

src = [
    'src/main.cpp',
    'src/impl.cpp',
    'src/app.cpp',
    'src/world.cpp',
    'src/manager/input_manager.cpp',
    'src/manager/window_manager.cpp',
    'src/manager/physics_manager.cpp',
    'src/manager/device_manager.cpp',
    'src/manager/material_manager.cpp',
    'src/manager/model_manager.cpp',
    'src/manager/ui_manager.cpp',
    'src/systems/transform_system.cpp',
    'src/systems/render_system.cpp',
    'src/systems/player_system.cpp',
    'src/systems/terrain_system.cpp'
]

reach_link_args = []

# if meson.is_cross_build()
#    reach_link_args += ['-sUSE_GLFW=3', '-sFULL_ES3']
# endif

executable('reach',
    src + shader_make_target,
    dependencies: deps,
    include_directories: ['src/'],
    override_options : ['cpp_std=c++20', 'cpp_eh=none', 'cpp_rtti=false', 'werror=true', 'warning_level=2'],
    cpp_args : ['-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING', '-D_HAS_EXCEPTIONS=0', '-DGLFW_INCLUDE_NONE', '-DGLM_ENABLE_EXPERIMENTAL', '-DSPDLOG_NO_EXCEPTIONS', '-DENTT_NOEXCEPTION'],
    link_args : reach_link_args)
