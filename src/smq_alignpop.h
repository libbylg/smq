#if !defined(SMQ_ALIGN_PUSH)
#error  "在 include smq_alignpop.h 之前必须先 include smq_alignpush.h"
#else
#undef  SMQ_ALIGN_PUSH
#endif


#if defined(SMQ_ALIGN_POP)
#error  "smq_alignpush.h 和 smq_alignpop.h 不允许嵌套使用"
#endif


#if defined(WIN32) || defined(WIN64)
#pragma pack(pop)
#endif


#define SMQ_ALIGN_POP

