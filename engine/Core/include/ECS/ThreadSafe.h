#pragma once

#define __THREAD_SAFE SAFE

#define SAFE  1
#define UNSAFE 2

#if __THREAD_SAFE == SAFE
#define THREAD_SAFE
#undef THREAD_UNSAFE
#else  
#define THREAD_UNSAFE
#undef THREAD_SAFE
#endif
