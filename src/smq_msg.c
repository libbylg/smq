#include "smq_msg.h"
#include "smq_os.h"
#include "smq_libc.h"
#include "smq_asserts.h"
#include "smq_inst.h"
#include "smq_errors.h"



static smq_int32    smq_alloc_queue_index(smq_t* smq, smq_uint32 size)
{
    smq_uint32 expect_size = size + sizeof(smq_block_t);

    for (smq_uint32 i = 0; i < smq->entry->alloc_queues_count; i++)
    {
        smq_alloc_queue_t* queue = smq->alloc_queues[i];
        if (expect_size <= queue->block_size)
        {
            return i;
        }
    }

    return (smq->entry->alloc_queues_count - 1);
}



static  smq_msg  smq_queue_pickup_first(smq_t* smq, smq_alloc_queue_t* queue)
{
    if (SMQ_MSG_NULL == queue->idle_block_last)
    {
        return SMQ_MSG_NULL;
    }

    smq_msg msg = SMQ_MSG_NULL;

    //  如果队列不为空
    smq_block_t* last  = (smq_block_t*)SMQ_ADDRESS_OF(smq, queue->idle_block_last);
    smq_block_t* first = (smq_block_t*)SMQ_ADDRESS_OF(smq, last->next);
    if (last == first)
    {
        msg = queue->idle_block_last;
        queue->idle_block_last = SMQ_MSG_NULL;
        last->next = SMQ_MSG_NULL;
    }else
    {
        msg = last->next;
        last->next = first->next;
    }

    return msg;
}



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_new(smq_inst inst, smq_uint32 size, smq_msg* msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_NEW;
    }

    smq_int32 primary_index = smq_alloc_queue_index(smq, size);
    for (smq_int32 i = primary_index; i >= 0; i--)
    {
        //  如果队列为空
        smq_alloc_queue_t* queue = smq->alloc_queues[i];
        smq_msg new_msg = smq_queue_pickup_first(smq, queue);
        if (SMQ_MSG_NULL != new_msg)
        {
            *msg = new_msg;
            return SMQ_OK;
        }
    }

    for (smq_int32 i = primary_index + 1; i < smq->entry->alloc_queues_count; i++)
    {
        //  如果队列为空
        smq_alloc_queue_t* queue = smq->alloc_queues[i];
        smq_msg new_msg = smq_queue_pickup_first(smq, queue);
        if (SMQ_MSG_NULL != new_msg)
        {
            *msg = new_msg;
            return SMQ_OK;
        }
    }

    return  SMQ_ERR_NO_ENOUGH_SHARE_MEMORY;
}




static  smq_msg  smq_queue_append_last(smq_t* smq, smq_msg msg)
{
    smq_block_t* cur_block = (smq_block_t*)SMQ_ADDRESS_OF(smq, msg);

    //  找到本块所属的队列
    smq_alloc_queue_t* queue = (smq_alloc_queue_t*)smq->alloc_queues[cur_block->queue_index];

    //  找到本队列的最后一个块，然后将当前要回收的块连接上去
    smq_block_t* queue_last = (smq_block_t*)SMQ_ADDRESS_OF(smq, queue->idle_block_last);
    cur_block->next  = queue_last->next;
    queue_last->next = msg;

    //  修改当前队列的最后一个块的指针
    queue->idle_block_last = msg;
    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_del(smq_inst inst, smq_msg msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_DEL;
    }


    //  追加到队尾
    smq_block_t* cur_block = (smq_block_t*)SMQ_ADDRESS_OF(smq, msg);
//     for (smq_block_t* sub = (smq_block_t*)SMQ_ADDRESS_OF(smq, cur_block->next); 
//         (SMQ_MSG_NULL != cur_block->next) || (sub->next == cur_block->next); 
//         sub = (smq_block_t*)SMQ_ADDRESS_OF(smq, sub->next))
//     {
// 
//     }

    //  如果该消息没有子消息
    if (SMQ_MSG_NULL == cur_block->next)
    {
        smq_queue_append_last(smq, msg);
        return SMQ_OK;
    }

    //  如果该消息有子消息
    smq_block_t* sub_last  = (smq_block_t*)SMQ_ADDRESS_OF(smq, cur_block->next);
    smq_block_t* sub_first = (smq_block_t*)SMQ_ADDRESS_OF(smq, sub_last->next);

    //  如果只有一个子消息
    if (sub_first == sub_last)
    {
        smq_queue_append_last(smq, cur_block->next);
        return SMQ_OK;
    }

    do
    {
        smq_block_t* pos  = sub_first;
        smq_block_t* next = (smq_block_t*)SMQ_ADDRESS_OF(smq, pos->next);
        smq_queue_append_last(smq, pos);
    }while(pos != sub_last)

    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_merge(smq_inst inst, smq_msg msg, smq_msg sub)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != sub),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_MERGE;
    }

    smq_block_t* cur_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);
    smq_block_t* sub_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub);
    
    //  还没有子消息
    if (SMQ_MSG_NULL == cur_block->next)
    {
        cur_block->next = sub;
        sub_block->next = sub;
        return SMQ_OK;
    }

    //  重新构造子消息环
    smq_block_t* last = (smq_block_t*)SMQ_ADDRESS_OF(inst, cur_block->next);
    sub_block->next = last->next;
    last->next      = sub;

    //  重置队尾指针
    cur_block->next     = sub;

    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((NULL          != next), "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    smq_block_t* cur_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    //  如果msg没有子元素或者已经到达消息链的尾部（注意这两个条件的判断方式是一模一样的）
    if (cur_block->next == *next)
    {
        *next = SMQ_MSG_NULL;
        return  SMQ_OK;
    }

    //  如果是第一次迭代
    smq_block_t* last = (smq_block_t*)(SMQ_ADDRESS_OF(inst, cur_block->next));
    if (SMQ_MSG_NULL == *next)
    {
        *next = last->next;
        return  SMQ_OK;
    }
   
    *next = cur_block->next;
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
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_FIX;
    }

    smq_block_t* block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    smq_uint32 cap  = smq->alloc_queues[block->queue_index]->block_size - sizeof(smq_block_t);
    if (len > cap)
    {
        return SMQ_ERR_LEN_LARGE_THAN_CAP;
    }

    return  SMQ_OK;
}


