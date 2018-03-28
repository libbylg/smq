#include "smq_msg.h"
#include "smq_os.h"
#include "smq_layout.h"
#include "smq_libc.h"
#include "smq_asserts.h"
#include "smq_inst.h"
#include "smq_errors.h"


SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_new(smq_inst inst, smq_uint32 size, smq_msg* msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;


    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_del(smq_inst inst, smq_msg* msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");



    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_merge(smq_inst inst, smq_msg msg, smq_msg sub)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");
    smq_t* smq = (smq_t*)inst;
    smq_block_t* block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);
    
    if (SMQ_MSG_NULL == block->next)
    {
        //block->next = 
    }
    else
    {

    }

    smq_block_t* last = SMQ_MSG_NULL;
    
    block->next;

    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((NULL          != next), "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    smq_block_t* block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);
    *next = (SMQ_MSG_NULL == block->next)?SMQ_MSG_NULL:block->next;

    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_data(smq_inst inst, smq_msg msg, smq_void** data, smq_uint32* len, smq_uint32* cap)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    smq_block_t* block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    smq_uint32 count = 0;
    if (NULL != data)
    {
        *data = block->data;
        count++;
    }
    
    if (NULL != len)
    {
        *len  = block->data_size;
        count++;
    }
    
    if (NULL != cap)
    {
        *cap  = smq->alloc_queues[block->queue_index]->block_size - sizeof(smq_block_t);
        count++;
    }

    SMQ_ASSERT((count > 0), "data、len、cap 三个参数均为 NULL，这种用法是错误的，通常可能是代码存在什么 bug");

    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_fix(smq_inst inst, smq_msg msg, smq_uint32 len)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    smq_block_t* block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    smq_uint32 cap  = smq->alloc_queues[block->queue_index]->block_size - sizeof(smq_block_t);
    if (len > cap)
    {
        return SMQ_ERR_LEN_LARGE_THAN_CAP;
    }

    return  SMQ_OK;
}


