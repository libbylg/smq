#include "smq_inst.h"
#include "smq_libc.h"
#include "smq_errors.h"
#include "smq_params.h"

static smq_errno   SMQ_CALL    smq_layout_load(smq_t* smq)
{
    
}

SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_close(smq_t* smq)
{
    if (NULL == smq)
    {
        return;
    }

    smq_shm_close(&(smq->shm));
    
    if (NULL != smq->alloc_groups)
    {
        smq_free(smq->alloc_groups);
    }

    if (NULL != smq->message_group)
    {
        smq_free(smq->message_group);
    }

    smq_free(smq);
}

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_open(smq_char* name, smq_uint32 role, smq_uint32 size, smq_inst* inst)
{
    SMQ_ASSERT((NULL != name), "名字不能为空");
    SMQ_ASSERT(((role < SMQ_ROLE_MIN) || (role > SMQ_ROLE_MAX)), "权限为空");

    smq_t* smq = (smq_t*)smq_malloc(sizeof(smq_t));
    if (NULL == smq)
    {
        return SMQ_ERR_MALLOC_FAILED;
    }

    smq_memset(smq, 0, sizeof(smq_t));

    smq_errno err = smq_shm_open(name, (smq_params.memory_size * 1024 * 1024), &(smq->shm));
    if (SMQ_OK != err)
    {
        return SMQ_ERR_INIT_OPEN_OR_CREATE_SHM_FAILED;
    }

    err = smq_layout_load(smq);
    if (SMQ_OK != err)
    {
        smq_shm_close(&(smq->shm));
        return SMQ_ERR_INIT_LAYOUT_FAILED;
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
