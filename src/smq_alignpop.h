#if !defined(SMQ_ALIGN_PUSH)
#error  "�� include smq_alignpop.h ֮ǰ������ include smq_alignpush.h"
#else
#undef  SMQ_ALIGN_PUSH
#endif


#if defined(SMQ_ALIGN_POP)
#error  "smq_alignpush.h �� smq_alignpop.h ������Ƕ��ʹ��"
#endif


#if defined(WIN32) || defined(WIN64)
#pragma pack(pop)
#endif


#define SMQ_ALIGN_POP

