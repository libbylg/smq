#include "smq_errors.h"


SMQ_EXTERN  smq_error_t smq_errors[SMQ_ERR_COUNT] = 
{
#define SMQ_ERR(name,id,cn,en)  {name, {sizeof(en), sizeof(cn)}, {en, cn}},
#define SMQ_LOG(name,id,cn,en)
#include    "smq_res.h"
#undef  SMQ_ERR
#undef  SMQ_LOG
};


SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_error(smq_errno err, smq_uint32 loc, smq_value_t* desc)
{
    if ((err < SMQ_ERR_MIN) || (err > SMQ_ERR_MAX))
    {
        return  SMQ_ERR_UNDEFINED_ERR;
    }

    if ((loc < SMQ_LOCALE_MIN) || (loc > SMQ_LOCALE_MAX))
    {
        return SMQ_ERR_UNDEFINED_LOC;
    }

    return  SMQ_OK;
}
