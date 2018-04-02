#ifndef __smq_asserts_H_
#define __smq_asserts_H_


/// 定义了通用的断言宏
#if defined(DEBUG)
#define SMQ_ASSERT(expr,m)    //assert(expr)
#else
#define SMQ_ASSERT(expr,m)
#endif


#endif//__smq_asserts_H_

