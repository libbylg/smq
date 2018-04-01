#if   defined(WIN32) || defined(WIN64)
#include "os/win.msvc/os.h"
#elif defined(__linux)
#include "os/linux.gcc/os.h"
#elif defined(__clang__)
#include "os/osx.clang/os.h"
#else
#error "Unsupported os or platform"
#endif
