#if   defined(WIN32) || defined(WIN64)
#include "os/win.msvc/os.c.inc"
#elif defined(__linux)
#include "os/linux.gcc/os.c.inc"
#elif defined(__clang__)
#include "os/osx.clang/os.h"
#else
#error "Unsupported os or platform"
#endif
