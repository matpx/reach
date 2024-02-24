project('reach', 'c', 'cpp', default_options: ['cpp_std=c++20'])

cmake = import('cmake')

glfw_proj = cmake.subproject('glfw')
glfw_lib = glfw_proj.dependency('glfw')

src = [
    'src/main.cpp',
    'src/impl.c',
    'src/app.cpp',
    'src/window.cpp',
    'src/renderer.cpp'
]

executable('reach', src, include_directories: ['subprojects'], dependencies: [glfw_lib])
