#include "smq_msg.h"
#include "smq_os.h"
#include "smq_libc.h"
#include "smq_asserts.h"
#include "smq_inst.h"
#include "smq_errors.h"
#include "smq_logs.h"


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



static  smq_msg  smq_queue_pop_front(smq_t* smq, smq_alloc_queue_t* queue)
{
    if (SMQ_MSG_NULL == queue->idle_block_last)
    {
        return SMQ_MSG_NULL;
    }

    smq_msg msg = SMQ_MSG_NULL;

    //  如果队列不为�?
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



static  smq_msg  smq_queue_push_back(smq_t* smq, smq_msg msg)
{
    smq_block_t* cur_block = (smq_block_t*)SMQ_ADDRESS_OF(smq, msg);

    //  找到本块所属的队列
    smq_alloc_queue_t* queue = (smq_alloc_queue_t*)smq->alloc_queues[cur_block->queue_index];

    //  找到本队列的最后一个块，然后将当前要回收的块连接上�?
    smq_block_t* queue_last = (smq_block_t*)SMQ_ADDRESS_OF(smq, queue->idle_block_last);
    cur_block->next  = queue_last->next;
    queue_last->next = msg;

    //  修改当前队列的最后一个块的指�?
    queue->idle_block_last = msg;
    return SMQ_OK;
}




/// 推进迭代器，但是返回推进之前的消�?
static smq_msg smq_itr_advance(smq_t* smq, smq_msg* itr)
{
    smq_msg ret = *itr;
    *itr = ((smq_block_t*)SMQ_ADDRESS_OF(smq, *itr))->next;
    return ret;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_new(smq_inst inst, smq_uint32 size, smq_msg* msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效�?);
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效�?);

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
        smq_msg new_msg = smq_queue_pop_front(smq, queue);
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
        smq_msg new_msg = smq_queue_pop_front(smq, queue);
        if (SMQ_MSG_NULL != new_msg)
        {
            *msg = new_msg;
            return SMQ_OK;
        }
    }

    return  SMQ_ERR_NO_ENOUGH_SHARE_MEMORY;
}




SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_del(smq_inst inst, smq_msg msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效�?);
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效�?);

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        SMQ_WARN(SMQ_LOG_READONLY_INST_UNSUPPORT_MSG_DEL, "msg=0x%x", msg);
        return;
    }

    //  如果该消息有子消息，那么需要先删除子消�?
    smq_block_t* block = (smq_block_t*)SMQ_ADDRESS_OF(smq, msg);
    if (SMQ_MSG_NULL != block->next)
    {
        //  如果该消息有子消息，那么从第一个消息开始删除多个消息：last->next 为第一个子消息
        smq_block_t* last  = (smq_block_t*)SMQ_ADDRESS_OF(smq, block->next);
        for (smq_msg pos = last->next; pos != block->next; )
        {
            smq_queue_push_back(smq, smq_itr_advance(smq, &pos));
        }

        //  接着删除最后一个子消息
        smq_queue_push_back(smq, block->next);
    }

    //  最后删�?msg 自身
    smq_queue_push_back(smq, msg);

    return;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_cat(smq_inst inst, smq_msg msg, smq_msg sub)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效�?);
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效�?);
    SMQ_ASSERT((SMQ_MSG_NULL  != sub),  "关键输入参数，由外部保证有效�?);

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_MERGE;
    }

    smq_block_t* msg_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);
    smq_block_t* sub_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub);
    
    //  还没有子消息
    if (SMQ_MSG_NULL == msg_block->next)
    {
        if (SMQ_MSG_NULL == sub_block->next)
        {
            msg_block->next = sub;  ///<    先修改msg的指针：sub就是最后添加的指针

            //  整理子消息环
            sub_block->next = sub;
        }
        else
        {
            msg_block->next = sub_block->next;  ///<    先修改msg的指针：sub_block->next 是新消息链的最后一个消�?

            //  整理子消息环
            smq_block_t* sub_last_block  = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub_block->next);
            sub_block->next = sub_last_block->next;
            sub_last_block->next = sub;     ///<    sub自身将成为msg的第一个消�?
        }
        
        return SMQ_OK;
    }

    //  拼接子消息环
    smq_block_t* msg_last_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg_block->next);
    if (SMQ_MSG_NULL == sub_block->next)
    {
        msg_block->next = sub;  ///<    先修改msg的指针：sub就是最后添加的指针

        //  整理子消息环
        sub_block->next = msg_last_block->next;
        msg_last_block->next = sub;
    }
    else
    {
        msg_block->next = sub_block->next;  ///<    先修改msg的指针：sub_block->next 是新消息链的最后一个消�?

        //  整理子消息环
        smq_block_t* sub_last_block  = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub_block->next);
        sub_block->next = sub_last_block->next;
        sub_last_block->next = msg_last_block->next;
    }
    
    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效�?);
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效�?);
    SMQ_ASSERT((NULL          != next), "关键输入参数，由外部保证有效�?);

    smq_t* smq = (smq_t*)inst;
    smq_block_t* cur_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    //  如果msg没有子元素或者已经到达消息链的尾部（注意这两个条件的判断方式是一模一样的�?
    if (cur_block->next == *next)
    {
        *next = SMQ_MSG_NULL;
        return  SMQ_OK;
    }

    //  如果是第一次迭�?
    smq_block_t* last = (smq_block_t*)(SMQ_ADDRESS_OF(inst, cur_block->next));
    if (SMQ_MSG_NULL == *next)
    {
        *next = last->next;
        return  SMQ_OK;
    }
   
    *next = cur_block->next;
    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_data(smq_inst inst, smq_msg msg, smq_void** data, smq_uint32* len, smq_uint32* cap)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效�?);
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效�?);

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

    SMQ_ASSERT((count > 0), "data、len、cap 三个参数均为 NULL，这种用法是错误的，通常可能是代码存在什�?bug");

    return;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_fix(smq_inst inst, smq_msg msg, smq_uint32 len)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效�?);
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效�?);

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



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_post(smq_inst inst, smq_msg msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效�?);
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效�?);

    smq_t* smq = (smq_t*)inst;

    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_wait(smq_inst inst, smq_int32 timeout, smq_msg* msg)
{
    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_peek(smq_inst inst, smq_uint32* count)
{
    return SMQ_OK;
}



