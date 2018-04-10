#include "smq_errors.h"
#include "smq_libc.h"




smq_error_t smq_errors[SMQ_ERR_COUNT] = 
{
#define SMQ_ERR(name,id,cn,en)  {name, {sizeof(en), sizeof(cn)}, {en, cn}},
#define SMQ_LOG(name,id,cn,en)
#include    "smq_res.h"
#undef  SMQ_ERR
#undef  SMQ_LOG
};




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_error(smq_errno err, smq_uint32 loc, smq_value_t* desc)
{
    //  错误码的上下限的值是占位用的
    if ((err <= SMQ_ERR_LOWER_LIMIT) || (err >= SMQ_ERR_UPPER_LIMIT))
    {
        return  SMQ_ERR_UNDEFINED_ERR;
    }

    if ((loc < SMQ_LOCALE_MIN) || (loc > SMQ_LOCALE_MAX))
    {
        return SMQ_ERR_UNDEFINED_LOC;
    }

    smq_error_t* error = &(smq_errors[err - SMQ_ERR_UPPER_LIMIT]);

    smq_uint32 copy_len = smq_min(error->len[loc], sizeof(smq_value_t));
    smq_memcpy(desc->value_str, error->desc[loc], copy_len);
    desc->value_str[sizeof(desc->value_str) - 1] = '\0';

    return  SMQ_OK;
}



