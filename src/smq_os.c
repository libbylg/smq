#if defined(WIN32) || defined(WIN64)
#include "os/win.msvc/os.c.inc"
#else
#include "os/linux.gcc/os.c.inc"
#endif
