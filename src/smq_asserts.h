#ifndef __smq_asserts_H_
#define __smq_asserts_H_


/// 定义了通用的断言宏
#if defined(DEBUG)
#define SMQ_ASSERT(expr,message)    assert
#else
#define SMQ_ASSERT(expr,message)
#endif


#endif//__smq_asserts_H_

