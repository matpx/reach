project('reach', 'c', 'cpp')

cmake = import('cmake')

glfw_proj = cmake.subproject('glfw')
glfw_lib = glfw_proj.dependency('glfw')

src = ['src/main.cpp', 'src/impl.c', 'src/app.cpp', 'src/window.cpp']

executable('reach', src, include_directories: [], dependencies: [glfw_lib])
