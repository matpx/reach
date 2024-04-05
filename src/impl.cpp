// msvc

#define _CRT_SECURE_NO_WARNINGS

// sokol

#ifdef __EMSCRIPTEN__
#define SOKOL_GLES3
#else
#define SOKOL_GLCORE33
#endif

#define SOKOL_IMPL
#include <sokol_gfx.h>

// cgltf

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

// mc

#define MC_IMPLEMENTATION
#include <rjm_mc.h>
