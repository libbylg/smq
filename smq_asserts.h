#ifndef __smq_asserts_H_
#define __smq_asserts_H_


/// ������ͨ�õĶ��Ժ�
#if defined(DEBUG)
#define SMQ_ASSERT(expr,m)    //assert(expr)
#else
#define SMQ_ASSERT(expr,m)
#endif


#endif//__smq_asserts_H_

