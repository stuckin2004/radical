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
    RAD_GFX_BACKEND_GLES20,             // OpenGL ES 2.0 (GLSL ES 100)
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

// Vertex layouts
//
// A vertex layout describes how vertex attributes are packed in a vertex
// buffer. The design is conservative on purpose: D3D8 fixed-function FVF and
// GL 2.1 / GLES 2.0 generic attributes both have to map cleanly onto this.
//
// - Max 8 attributes per layout (D3D8 FVF and GLES2 both honor this comfortably).
// - Interleaved-only. No separate streams. D3D8 supports multi-stream but GLES2
//   wants one VBO per attribute or interleaved; interleaved-only is the lowest
//   common denominator.
// - `semantic` exists so the D3D8 backend can build an FVF and the GL backends
//   can bind to a known attribute slot. Shaders in GL declare attribs by
//   semantic name (e.g. "a_position") via rad_gfx_shader_bind_attrib below.

#define RAD_GFX_MAX_VERTEX_ATTRIBS 8
#define RAD_GFX_MAX_VERTEX_STRIDE 256

typedef enum rad_gfx_attrib_semantic {
    RAD_GFX_ATTR_POSITION = 0,
    RAD_GFX_ATTR_NORMAL,
    RAD_GFX_ATTR_TANGENT,
    RAD_GFX_ATTR_COLOUR0,
    RAD_GFX_ATTR_COLOUR1,
    RAD_GFX_ATTR_TEXCOORD0,
    RAD_GFX_ATTR_TEXCOORD1,
    RAD_GFX_ATTR_TEXCOORD2,
    RAD_GFX_ATTR_TEXCOORD3,
} rad_gfx_attrib_semantic;

typedef enum rad_gfx_attrib_type {
    RAD_GFX_ATYPE_FLOAT32 = 0,
    RAD_GFX_ATYPE_INT16,        // signed, can be normalized
    RAD_GFX_ATYPE_UINT16,       // unsigned, can be normalized
    RAD_GFX_ATYPE_INT8,
    RAD_GFX_ATYPE_UINT8,        // common for colours when normalized
} rad_gfx_attrib_type;

typedef struct rad_gfx_vertex_attrib {
    rad_gfx_attrib_semantic semantic;
    rad_gfx_attrib_type     type;
    uint8_t                 components;   // 1..4
    uint8_t                 offset;       // bytes from start of vertex
    bool                    normalized;   // ignored for FLOAT32
} rad_gfx_vertex_attrib;

typedef struct rad_gfx_vertex_layout {
    uint32_t              stride;         // bytes per vertex
    uint32_t              attrib_count;   // <= RAD_GFX_MAX_VERTEX_ATTRIBS
    rad_gfx_vertex_attrib attribs[RAD_GFX_MAX_VERTEX_ATTRIBS];
} rad_gfx_vertex_layout;

// Index format; D3D8 supports both 16- and 32-bit indices; GLES2 base spec
// is 16-bit only and gets 32-bit via OES_element_index_uint. Backends should
// fall back to 16-bit + log a warning if 32-bit isn't supported.
typedef enum rad_gfx_index_type {
    RAD_GFX_INDEX_NONE = 0,
    RAD_GFX_INDEX_U16,
    RAD_GFX_INDEX_U32,
} rad_gfx_index_type;

// Hardware descriptors
// 
// "Descriptor" here means the create-info struct passed to a resource creation
// call. Naming follows Vulkan-ish convention since we have a `vk` backend dir
// stubbed out, but these stay small and POD-friendly so D3D8 can map them
// trivially to its API.
//
// All descriptors are zero-initializable. Setting a field to 0 = sensible
// default (e.g. zero usage = STATIC, zero filter = NEAREST). This keeps
// `(rad_gfx_texture_desc){ .width = 64, .height = 64, .format = ... }` ergonomic.

typedef struct rad_gfx_texture_desc {
    uint32_t             width;
    uint32_t             height;
    rad_gfx_pixel_format format;
    uint32_t             mip_levels;       // 0 or 1 = no mips, >1 = explicit count
    rad_gfx_filter       min_filter;
    rad_gfx_filter       mag_filter;
    rad_gfx_wrap         wrap_u;
    rad_gfx_wrap         wrap_v;
    bool                 render_target;     // can be attached to a rad_gfx_target
    const void* initial_data;               // NULL = uninitialized (legal for RTs)
    size_t               initial_data_size;
} rad_gfx_texture_desc;

typedef struct rad_gfx_buffer_desc {
    rad_gfx_buffer_kind kind;
    rad_gfx_usage       usage;
    size_t              size;               // bytes
    const void* initial_data;               // may be NULL
    
    // Only used when kind == RAD_GFX_BUF_INDEX. Ignored otherwise.
    rad_gfx_index_type  index_type;
} rad_gfx_buffer_desc;

typedef struct rad_gfx_shader_desc {
    // GLSL 120 / GLSL ES 100 / D3D8 asm or HLSL fragments. Backends are
    // expected to receive source already preprocessed for their target;
    // a higher-level shader cooker handles the cross-compile.
    const char* vertex_source;
    size_t      vertex_source_size;        // 0 = strlen()
    const char* fragment_source;
    size_t      fragment_source_size;      // 0 = strlen()

    // Layout the shader expects. The backend uses this to wire up attribute
    // locations (glBindAttribLocation on GL, FVF on D3D8) before linking.
    rad_gfx_vertex_layout layout;
} rad_gfx_shader_desc;

typedef struct rad_gfx_target_desc {
    // Up to one color attachment for now. GL 2.1 + GLES2 base spec only
    // guarantee one; MRT needs extensions and we're staying conservative.
    rad_gfx_texture color;                 // must have render_target = true
    rad_gfx_texture depth_stencil;         // optional, pass invalid handle to skip
    uint32_t        width;
    uint32_t        height;
} rad_gfx_target_desc;

// Pipeline-ish state. D3D8 and GL2 are both state-machine APIs so this is
// just a bag of state we apply before draws rather than a baked PSO.
typedef struct rad_gfx_pipeline_state {
    rad_gfx_blend       blend;
    rad_gfx_cull        cull;
    rad_gfx_depth_func  depth_func;
    bool                depth_write;
    bool                scissor_enabled;
    int32_t             scissor_x, scissor_y;
    uint32_t            scissor_w, scissor_h;
} rad_gfx_pipeline_state;

// Lifecycle functions
//
// These deal in the general life of the graphics system, handling
// textures, buffers, targets, shaders and everything else we need.

rad_gfx_texture rad_gfx_texture_create(const rad_gfx_texture_desc* desc);
rad_gfx_buffer  rad_gfx_buffer_create(const rad_gfx_buffer_desc* desc);
rad_gfx_shader  rad_gfx_shader_create(const rad_gfx_shader_desc* desc);
rad_gfx_target  rad_gfx_target_create(const rad_gfx_target_desc* desc);

void rad_gfx_texture_destroy(rad_gfx_texture h);
void rad_gfx_buffer_destroy(rad_gfx_buffer  h);
void rad_gfx_shader_destroy(rad_gfx_shader  h);
void rad_gfx_target_destroy(rad_gfx_target  h);

// Global lifecycle funcs

void        rad_gfx_shutdown(void);
const char* rad_gfx_last_error(void);
void        rad_gfx_resize(uint32_t width, uint32_t height);

#ifdef __cplusplus
}
#endif

#endif /* RAD_GRAPHICS_H */
