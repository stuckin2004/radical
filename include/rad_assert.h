#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

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

#ifndef __cplusplus
}
#endif