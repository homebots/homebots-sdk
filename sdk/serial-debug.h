#ifndef _DEBUG_MACRO_
#define _DEBUG_MACRO_

#ifdef DEBUG
#define LOG(...) os_printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#endif
