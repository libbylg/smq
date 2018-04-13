#if   defined(_WIN32)  && defined(_MSC_VER)
#include "os/win.msvc/os.c"
#elif defined(_WIN32)  && defined(__GNUC__)
#include "os/win.mingw/os.c"
#elif defined(_WIN32)  && defined(__clang__)
#include "os/win.clang/os.c"
#elif defined(__linux) && defined(__GNUC__)
#include "os/linux.gcc/os.c"
#else
#error "(os.c.inc)Unsupported compiler or platform"
#endif
