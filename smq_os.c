#if   defined(_WIN32) && defined(_MSC_VER)
#include "os/win.msvc/os.c.inc"
#elif defined(_WIN32) && defined(__GNUC__)
#include "os/linux.gcc/os.c.inc"
#elif defined(_WIN32) && defined(__clang__)
#include "os/osx.clang/os.c.inc"
#else
#error "(os.c)Unsupported compiler or platform"
#endif
