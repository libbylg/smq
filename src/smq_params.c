#include "smq_params.h"
#include "smq_limit.h"
#include "smq_libc.h"
#include "smq_asserts.h"
#include "smq_errors.h"

static smq_void   SMQ_CALL default_log_func(smq_void* context, smq_uint32 id, smq_uint32 level, smq_uint32 loc, smq_char* desc, smq_uint32 desc_len, smq_char* synamic, smq_uint32 synamic_len);

smq_params_t    smq_params = 
{
    SMQ_LOG_LEVEL_DEF,
    NULL,
    default_log_func,
    SMQ_MEMORY_SIZE_DEF,
    SMQ_MESSAGE_QUEUE_SIZE_DEF,
};

static smq_void   SMQ_CALL default_log_func(smq_void* context, smq_uint32 id, smq_uint32 level, smq_uint32 loc, smq_char* desc, smq_uint32 desc_len, smq_char* dynamic, smq_uint32 dynamic_len)
{
    static smq_char* level_name[(SMQ_LOG_LEVEL_MAX - SMQ_LOG_LEVEL_MIN) + 1] = 
    {
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
    };

    smq_printf("%s|%d|%s|%s\n", level_name[level], id, desc, dynamic);
    return;
}

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_get(smq_uint32 key, smq_value_t* val)
{
    SMQ_ASSERT((NULL != val), "关键参数由外部保证参数正确性");

    if ((key < SMQ_PARAM_KEY_MIN) || (key > SMQ_PARAM_KEY_MAX))
    {
        return SMQ_ERR_PARAM_UNSUPPORTED_KEY;
    }

    switch (key)
    {
    case SMQ_PARAM_LOG_LEVEL:
        val->value_uint32   =   smq_params.log_level;   
        return  SMQ_OK;
    case SMQ_PARAM_MEMORY_SIZE:
        val->value_uint32   =   smq_params.memory_size;
        return  SMQ_OK;
    case SMQ_PARAM_QUEUE_SIZE:
        val->value_uint32   =   smq_params.queue_size;
        return  SMQ_OK;
    case SMQ_PARAM_SUPPORTED_VERSIONS:  
        val->value_uint32s[0]   =   SMQ_VERSION_MIN;
        val->value_uint32s[2]   =   SMQ_VERSION_MAX;
        return  SMQ_OK;
    case SMQ_PARAM_LOCALE:
        val->value_uint32   =   smq_params.locale;      
        return  SMQ_OK;  
    case SMQ_PARAM_LOG_TARGET:
    default:
        return SMQ_ERR_PARAM_UNSUPPORTED_GET_KEY;
    }
}

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_set(smq_uint32 key, smq_value_t* val)
{
    SMQ_ASSERT((NULL != val), "关键参数由外部保证参数正确性");

    //  设置之前先校验一下
    smq_errno err = smq_param_check(key, val);
    if (err != SMQ_OK)
    {
        return err;
    }


    //  执行具体的设置工作
    switch (key)
    {
    case SMQ_PARAM_LOG_LEVEL:
        smq_params.log_level    =   val->value_uint32;   
        return  SMQ_OK;
    case SMQ_PARAM_MEMORY_SIZE:
        smq_params.memory_size  =   val->value_uint32;
        return  SMQ_OK;
    case SMQ_PARAM_QUEUE_SIZE:
        smq_params.queue_size   =   val->value_uint32;
        return  SMQ_OK;
    case SMQ_PARAM_LOCALE:
        smq_params.locale       =   val->value_uint32;      
        return  SMQ_OK;
    case SMQ_PARAM_LOG_TARGET:
        smq_params.log_context  =   val->value_ptrs[0];
        smq_params.log_func     =   (SMQ_LOGGER_FUNC)(val->value_ptrs[1]);
        return SMQ_OK;
    case SMQ_PARAM_SUPPORTED_VERSIONS:
        return  SMQ_ERR_PARAM_UNSUPPORTED_SET_KEY;
    default:
        return SMQ_ERR_PARAM_UNSUPPORTED_GET_KEY;
    }
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_param_check(smq_uint32 key, smq_value_t* val)
{
    SMQ_ASSERT((NULL != val), "关键参数由外部保证参数正确性");

    if ((key < SMQ_PARAM_KEY_MIN) || (key > SMQ_PARAM_KEY_MAX))
    {
        return SMQ_ERR_PARAM_UNSUPPORTED_KEY;
    }

    switch (key)
    {
    case SMQ_PARAM_LOG_LEVEL:
        if ((val->value_uint32 < SMQ_LOG_LEVEL_MIN) || (val->value_uint32 < SMQ_LOG_LEVEL_MAX))
        {
            return  SMQ_ERR_LOG_LEVEL_OUT_OF_RANGE;
        }
        return  SMQ_OK;
    case SMQ_PARAM_MEMORY_SIZE:
        if ((val->value_uint32 < SMQ_MEMORY_SIZE_MIN) || (val->value_uint32 < SMQ_MEMORY_SIZE_MAX))
        {
            return  SMQ_ERR_MEMORY_SIZE_OUT_OF_RANGE;
        }
        return  SMQ_OK;
    case SMQ_PARAM_QUEUE_SIZE:
        if ((val->value_uint32 < SMQ_MESSAGE_QUEUE_SIZE_MIN) || (val->value_uint32 < SMQ_MESSAGE_QUEUE_SIZE_MAX))
        {
            return  SMQ_ERR_MESSAGE_QUEUE_SIZE_OUT_OF_RANGE;
        }
        return  SMQ_OK;
    case SMQ_PARAM_LOCALE:
        if ((val->value_uint32 < SMQ_LOCALE_MIN) || (val->value_uint32 < SMQ_LOCALE_MAX))
        {
            return  SMQ_ERR_LOCALE_OUT_OF_RANGE;
        }
        return  SMQ_OK;
    case SMQ_PARAM_LOG_TARGET:
        if (NULL == val->value_ptrs[1])
        {
            return  SMQ_ERR_LOG_FUNCTION_NULL;
        }
        return SMQ_OK;
    case SMQ_PARAM_SUPPORTED_VERSIONS:
        return  SMQ_ERR_PARAM_UNSUPPORTED_SET_KEY;
    default:
        return SMQ_ERR_PARAM_UNSUPPORTED_GET_KEY;
    }
}


