// rad_alloc.h -> special allocator tools for Radical in memory strict spaces
// Backend implementations live in "src/alloc/[arena,heap,pool].c"
#ifndef RAD_ALLOC_H
#define RAD_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Follows SemVer
#define RAD_ALLOC_API_VERSION 100

// Allocator vtable
//
// All allocators in Radical are reached through a `rad_allocator` handle that
// carries a vtable + opaque state pointer. Pass these around explicitly !
// No TLS, no globals (well, almost... see rad_alloc_default below).
//
// The vtable is intentionally minimal. Five entry points cover everything:
// alloc, realloc, free, reset, and destroy. Allocators that don't support
// an op (e.g. arenas can't free a single allocation) set the slot to NULL
// and the wrappers below will hit RAD_ASSERT in debug builds.
//
// All pointers returned are aligned to at least RAD_ALLOC_MIN_ALIGN unless
// the caller asks for more via the _aligned variants. Backends are free to
// over-align; they just can't under-align.

#define RAD_ALLOC_MIN_ALIGN 16 // SIMD-friendly, also fine for VBO data on most hardware

// Allocation tag, used for memory reporting; pass one in to EVERY alloc call
// and the allocator can keep totals for debug memory dumps.
typedef enum rad_alloc_tag {
    RAD_TAG_MISC = 0,
    RAD_TAG_GFX,
    RAD_TAG_AUDIO,
    RAD_TAG_ASSETS,
    RAD_TAG_SCRIPT,
    RAD_TAG_SCENE,
    RAD_TAG_NET,
    RAD_TAG_DEBUG,
    RAD_TAG_TEMP,
    RAD_TAG_COUNT,
} rad_alloc_tag;

typedef struct rad_alloc_stats {
    size_t bytes_in_use;            // currently live
    size_t bytes_high_water;        // peak since creation/reset
    size_t bytes_capacity;          // total reservation, 0 = unbounded
    size_t alloc_count;             // live allocation count
    size_t alloc_count_total;       // lifetime alloc calls (incl. freed)
    size_t bytes_per_tag[RAD_TAG_COUNT];
} rad_alloc_stats;

// The vtable. `self` is the allocator's opaque state, passed through verbatim.
// `tag` is hint-only; allocators are not required to honor it for placement,
// just for accounting.
typedef struct rad_allocator_vtable {
    void* (*alloc)  (void* self, size_t size, size_t align, rad_alloc_tag tag);
    void* (*realloc)(void* self, void* ptr, size_t old_size, size_t new_size,
        size_t align, rad_alloc_tag tag);
    void  (*free)   (void* self, void* ptr, size_t size);
    void  (*reset)  (void* self);                           // arena-style bulk reset
    void  (*destroy)(void* self);                           // tear down the allocator itself
    void  (*stats)  (void* self, rad_alloc_stats* out);     // optional, may be NULL
    const char* name;                                       // for debug dumps
} rad_allocator_vtable;

typedef struct rad_allocator {
    const rad_allocator_vtable* vt;
    void* self;
} rad_allocator;


#ifdef __cplusplus
}
#endif

#endif /* RAD_ALLOC_H */