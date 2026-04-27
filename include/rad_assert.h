#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>

// Due to the existence of __debugbreak() this is technically MSVC only (oops)
#define RAD_ASSERT(exp) \
	do { \
		if (!(exp)) { \
			fprintf(stderr, "Assert fail: (%s), func %s, file %s, line %d.\n", \
			#exp, __func__, __FILE__, __LINE__); \
			__debugbreak(); \
			abort(); \
		} \
	} while(0)

// This is a "runtime assertion" - basically the same as RAD_ASSERT but it won't
// crash the process, it just logs to stderr and moves on.
#define RAD_RTASSERT(exp) \
	do { \
		if (!(exp)) { \
			fprintf(stderr, "RTAssert fail: (%s), func %s, file %s, line %d.\n", \
			#exp, __func__, __FILE__, __LINE__); \
		} \
	} while(0)

#ifdef __cplusplus
}
#endif
