#if   defined(WIN32) || defined(WIN64)
#include "os/win.msvc/os.c.inc"
#elif defined(__linux)
#include "os/linux.gcc/os.c.inc"
#elif defined(__clang__)
#include "os/osx.clang/os.c.inc"
#else
#error "(os.c)Unsupported compiler or platform"
#endif
