#ifndef _DEBUG_MACRO_
#define _DEBUG_MACRO_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define LOG(...) os_printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#ifdef __cplusplus
}
#endif

#endif
