#if   defined(_WIN32)
#include "os/win.msvc/os.h"
#elif defined(__linux)
#include "os/linux.gcc/os.h"
#elif defined(__clang__)
#include "os/osx.clang/os.h"
#else
#error "(os.h)Unsupported compiler or platform"
#endif
