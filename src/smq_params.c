#include "smq_params.h"
#include "smq_limit.h"
#include "smq_libc.h"

smq_int32   SMQ_CALL default_log_func(smq_void* context, smq_uint32 id, smq_uint32 level, smq_uint32 loc, smq_char* desc, smq_uint32 desc_len, smq_char* synamic, smq_uint32 synamic_len);

extern  smq_params_t    smq_params = 
{
    SMQ_LOG_LEVEL_DEF,
    NULL,
    default_log_func,
    SMQ_MEMORY_SIZE_DEF,
    SMQ_MESSAGE_QUEUE_SIZE_DEF,
};

smq_int32   SMQ_CALL default_log_func(smq_void* context, smq_uint32 id, smq_uint32 level, smq_uint32 loc, smq_char* desc, smq_uint32 desc_len, smq_char* synamic, smq_uint32 synamic_len)
{
    return  0;
}

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_get(smq_uint32 key, smq_value_t* val)
{
    return  SMQ_OK;
}

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_set(smq_uint32 key, smq_value_t* val)
{
    return  SMQ_OK;
}