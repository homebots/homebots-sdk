#ifndef _DEBUG_MACRO_
#define _DEBUG_MACRO_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEBUG
#define LOG(...)
#else
#define LOG(...) os_printf(__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif
