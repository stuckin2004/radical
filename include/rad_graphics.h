// rad_graphics.h -> radical framework graphics frontend
// Backend impls live in "src/[gl21,d3d8,vk,...]/rad_graphics.c"
#ifndef RAD_GRAPHICS_H
#define RAD_GRAPHICS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// This version number follows semantic versioning
#define RAD_GFX_API_VERSION 100

typedef enum rad_gfx_backend {
    RAD_GFX_BACKEND_NULL = 0,           // Headless, good for CI tests
    RAD_GFX_BACKEND_SOFTWARE,           // CPU bound rendering
    RAD_GFX_BACKEND_GL21,               // OpenGL 2.1    (GLSL 120)
    RAD_GFX_BACKEND_GLE20,              // OpenGL ES 2.0 (GLSL ES 100)
    RAD_GFX_BACKEND_D3D8,               // Assumes D3D8, not 8.1 or any sub-version
} rad_gfx_backend;

typedef struct rad_gfx_caps {
    rad_gfx_backend backend;
    uint32_t        max_texture_size;      // px, square
    uint32_t        max_texture_uints;
    uint32_t        max_vertex_attributes;
    bool            npot_features;
    bool            mipmaps;
    bool            vsync_supported;
    bool            srgb_framebuffer;
    char            renderer_name[128];
} rad_gfx_caps;

// These are left intentionally opaque as they're backend-defined

typedef struct rad_gfx_texture { uint32_t id; } rad_gfx_texture;
typedef struct rad_gfx_buffer  { uint32_t id; } rad_gfx_buffer;
typedef struct rad_gfx_shader  { uint32_t id; } rad_gfx_shader;
typedef struct rad_gfx_target  { uint32_t id; } rad_gfx_target;

#define RAD_GFX_INVALID_HANDLE ((uint32_t)0)
#define RAD_GFX_IS_VALID(h)    ((h).id != RAD_GFX_INVALID_HANDLE)

// Various critical enums

typedef enum rad_gfx_pixel_format {
    RAD_GFX_PF_UNKNOWN = 0,
    RAD_GFX_PF_R8,
    RAD_GFX_PF_RG8,
    RAD_GFX_PF_RGB8,
    RAD_GFX_PF_RGBA8,
    RAD_GFX_PF_BGRA8,
    RAD_GFX_PF_DEPTH24_STENCIL8,
    RAD_GFX_PF_DEPTH16,
} rad_gfx_pixel_format;

typedef enum rad_gfx_primitive {
    RAD_GFX_PRIM_TRIANGLES = 0,
    RAD_GFX_PRIM_TRIANGLE_STRIP,
    RAD_GFX_PRIM_LINES,
    RAD_GFX_PRIM_LINE_STRIP,
    RAD_GFX_PRIM_POINTS,
} rad_gfx_primitive;

typedef enum rad_gfx_buffer_kind {
    RAD_GFX_BUF_VERTEX = 0,
    RAD_GFX_BUF_INDEX,
    RAD_GFX_BUF_UNIFORM,
} rad_gfx_buffer_kind;

typedef enum rad_gfx_usage {
    RAD_GFX_USAGE_STATIC = 0,
    RAD_GFX_USAGE_DYNAMIC,
    RAD_GFX_USAGE_STREAM,
} rad_gfx_usage;

typedef enum rad_gfx_filter {
    RAD_GFX_FILTER_NEAREST = 0,
    RAD_GFX_FILTER_LINEAR,
    RAD_GFX_FILTER_NEAREST_MIP_NEAREST,
    RAD_GFX_FILTER_LINEAR_MIP_LINEAR,
} rad_gfx_filter;

typedef enum rad_gfx_wrap {
    RAD_GFX_WRAP_REPEAT = 0,
    RAD_GFX_WRAP_CLAMP,
    RAD_GFX_WRAP_MIRROR,
} rad_gfx_wrap;

typedef enum rad_gfx_blend {
    RAD_GFX_BLEND_NONE = 0,
    RAD_GFX_BLEND_ALPHA,
    RAD_GFX_BLEND_ADDITIVE,
    RAD_GFX_BLEND_MULTIPLY,
    RAD_GFX_BLEND_PREMULTIPLIED,
} rad_gfx_blend;

typedef enum rad_gfx_cull {
    RAD_GFX_CULL_NONE = 0,
    RAD_GFX_CULL_BACK,
    RAD_GFX_CULL_FRONT,
} rad_gfx_cull;

typedef enum rad_gfx_depth_func {
    RAD_GFX_DEPTH_NONE = 0,
    RAD_GFX_DEPTH_LESS,
    RAD_GFX_DEPTH_LEQUAL,
    RAD_GFX_DEPTH_EQUAL,
    RAD_GFX_DEPTH_ALWAYS,
} rad_gfx_depth_func;

typedef enum rad_gfx_clear_flags {
    RAD_GFX_CLEAR_COLOR   = 1 << 0,
    RAD_GFX_CLEAR_DEPTH   = 1 << 1,
    RAD_GFX_CLEAR_STENCIL = 1 << 2,
} rad_gfx_clear_flags;

// TODO: Vertex layouts
//

// TODO: Hardware descriptors
//

// Lifecycle functions

void            rad_gfx_shutdown(void);
const char*     rad_gfx_last_error(void);
void            rad_gfx_resize(uint32_t width, uint32_t height);

#ifdef __cplusplus
}
#endif

#endif /* RAD_GRAPHICS_H */
