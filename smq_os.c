#if   defined(_WIN32) && defined(_MSC_VER)
#include "os/win.msvc/os.c.inc"
#elif defined(_WIN32) && defined(__GNUC__)
#include "os/win.gcc/os.c.inc"
#elif defined(_WIN32) && defined(__clang__)
#include "os/win.clang/os.c.inc"
#else
#error "(os.c.inc)Unsupported compiler or platform"
#endif
