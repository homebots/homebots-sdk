#ifndef _DEBUG_MACRO_
#define _DEBUG_MACRO_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEBUG
#define DEBUG(...) os_printf(__VA_ARGS__)
// #define DEBUG(...)
#endif

#ifdef __cplusplus
}
#endif

#endif
