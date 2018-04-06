#if   defined(_WIN32)  && defined(_MSC_VER)
#include "os/win.msvc/os.h"
#elif defined(_WIN32)  && defined(__GNUC__)
#include "os/win.mingw/os.h"
#elif defined(_WIN32)  && defined(__clang__)
#include "os/win.clang/os.h"
#elif defined(__linux) && defined(__GNUC__)
#include "os/linux.gcc/os.h"
#else
#error "(os.h)Unsupported compiler or platform"
#endif
