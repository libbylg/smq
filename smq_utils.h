#ifndef __smq_utils_H_
#define __smq_utils_H_

#include "smq.h"

SMQ_EXTERN  smq_uint16 SMQ_CALL smq_checksum(smq_uint8* data, smq_uint32 size);


// #if   defined(_MSC_VER)
// #pragma warning(disable:4200)   ///<    VC 编译器特殊处理
// #endif
// typedef struct  
// {
//     smq_uint32  cap;
//     smq_uint32  len;
//     smq_char    str[0];
// }smq_str_t;
// #if   defined(_MSC_VER)
// #pragma warning(default:4200)   ///<    VC 编译器特殊处理
// #endif
#endif//__smq_utils_H_

