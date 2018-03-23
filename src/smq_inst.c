#include "smq_inst.h"
#include "smq_libc.h"
#include "smq_errors.h"

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_init(smq_char* name, smq_uint32 role, smq_uint32 size, smq_inst* inst)
{
    smq_t* smq = (smq_t*)smq_malloc(sizeof(smq_t));
    if (NULL == smq)
    {
        return SMQ_ERR_MALLOC_FAILED;
    }

    return SMQ_OK;
}

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_finit(smq_char* name, smq_uint32 size, smq_inst* inst)
{

    return SMQ_OK;
}

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_version(smq_inst inst, smq_uint32* ver)
{
    return SMQ_OK;

}

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_dump(smq_inst inst, smq_uint32 range, smq_void* context, SMQ_DUMPER_FUNC f)
{

    return SMQ_OK;
}
