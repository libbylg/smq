#if defined(SMQ_ALIGN_PUSH)
#error  "在 include smq_alignpop.h 之前必须先 include smq_alignpush.h"
#endif



#if   defined(_MSC_VER)
#pragma pack(push, 1)
#endif



#if defined(SMQ_ALIGN_POP)
#undef  SMQ_ALIGN_POP
#endif


#define SMQ_ALIGN_PUSH



