#if   defined(_WIN32)  && defined(_MSC_VER)
#include "os/win.msvc/os.c.inc"
#elif defined(_WIN32)  && defined(__GNUC__)
#include "os/win.mingw/os.c.inc"
#elif defined(_WIN32)  && defined(__clang__)
#include "os/win.clang/os.c.inc"
#elif defined(__linux) && defined(__GNUC__)
#include "os/linux.gcc/os.c.inc"
#else
#error "(os.c.inc)Unsupported compiler or platform"
#endif
