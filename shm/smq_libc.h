#ifndef __smq_libc_H_
#define __smq_libc_H_


#include <stdlib.h>
#define smq_max(a,b)    (((a) > (b)) ? (a) : (b))
#define smq_min(a,b)    (((a) < (b)) ? (a) : (b))
#define smq_malloc      malloc
#define smq_free        free


#include <string.h>
#define smq_memcpy      memcpy
#define smq_memset      memset
#define smq_strlen      strlen


#include <stdio.h>
#if     defined(_MSC_VER)
#define smq_vsnprintf   vsprintf_s
#else
#define smq_vsnprintf   vsnprintf
#endif
#define smq_printf      printf


#include <stdarg.h>


#include <assert.h>
/// 定义了通用的断言宏
#if defined(DEBUG)
#define SMQ_ASSERT(expr,m)  assert(expr)
#else
#define SMQ_ASSERT(expr,m)
#endif

#endif

