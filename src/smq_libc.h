#ifndef __smq_libc_H_
#define __smq_libc_H_


#include <stdlib.h>
#define smq_malloc      malloc
#define smq_free        free


#include <string.h>
#define smq_memcpy      memcpy
#define smq_memset      memset
#define smq_strlen      strlen


#include <stdio.h>
#define smq_sprintf_s   sprintf_s

#endif