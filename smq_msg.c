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



inline  smq_msg  smq_queue_pop_front(smq_t* smq, smq_alloc_queue_t* queue)
{
    if (SMQ_MSG_NULL == queue->block_last)
    {
        return SMQ_MSG_NULL;
    }

    smq_msg msg = SMQ_MSG_NULL;

    //  如果队列不为空
    smq_block_t* last  = (smq_block_t*)SMQ_ADDRESS_OF(smq, queue->block_last);
    smq_block_t* first = (smq_block_t*)SMQ_ADDRESS_OF(smq, last->next);
    if (last == first)
    {
        msg = queue->block_last;
        queue->block_last = SMQ_MSG_NULL;
        last->next = SMQ_MSG_NULL;
    }
    else
    {
        msg = last->next;
        last->next = first->next;
    }

    return msg;
}



inline  smq_msg  smq_queue_push_back(smq_t* smq, smq_msg msg)
{
    smq_block_t* cur_block = (smq_block_t*)SMQ_ADDRESS_OF(smq, msg);

    //  找到本块所属的队列
    smq_alloc_queue_t* queue = (smq_alloc_queue_t*)smq->alloc_queues[cur_block->queue_index];

    //  找到本队列的最后一个块，然后将当前要回收的块连接上去
    smq_block_t* queue_last = (smq_block_t*)SMQ_ADDRESS_OF(smq, queue->block_last);
    cur_block->next  = queue_last->next;
    queue_last->next = msg;

    //  修改当前队列的最后一个块的指针
    queue->block_last = msg;
    return SMQ_OK;
}




/// 推进迭代器，但是返回推进之前的消息
inline smq_msg smq_itr_advance(smq_t* smq, smq_msg* itr)
{
    smq_msg ret = *itr;
    *itr = ((smq_block_t*)SMQ_ADDRESS_OF(smq, *itr))->next;
    return ret;
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
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        SMQ_WARN(SMQ_LOG_READONLY_INST_UNSUPPORT_MSG_DEL, "msg=0x%x", msg);
        return;
    }

    //  如果该消息有子消息，那么需要先删除子消息
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

    //  最后删除 msg 自身
    smq_queue_push_back(smq, msg);

    return;
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
            msg_block->next = sub_block->next;  ///<    先修改msg的指针：sub_block->next 是新消息链的最后一个消息

            //  整理子消息环
            smq_block_t* sub_last_block  = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub_block->next);
            sub_block->next = sub_last_block->next;
            sub_last_block->next = sub;     ///<    sub自身将成为msg的第一个消息
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
        msg_block->next = sub_block->next;  ///<    先修改msg的指针：sub_block->next 是新消息链的最后一个消息

        //  整理子消息环
        smq_block_t* sub_last_block  = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub_block->next);
        sub_block->next = sub_last_block->next;
        sub_last_block->next = msg_last_block->next;
    }
    
    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_NULL      != next), "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    smq_block_t* root = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    //  如果已经达到最后一个消息
    if (SMQ_MSG_NULL == *next)
    {
        return SMQ_ERR_END_OF_MSG_NO_NEXT;
    }

    //  如果找最后一个消息的下一个消息
    if (root->next == *next)
    {
        *next = SMQ_MSG_NULL;
        return SMQ_OK;
    }

    //  如果next指向msg，意味着用户实际想获取msg的第一个子消息
    if (msg == *next)
    {
        //  如果没有子消息
        if (SMQ_MSG_NULL == root->next)
        {
            *next = SMQ_MSG_NULL;
            return SMQ_OK;
        }

        smq_block_t* last = (smq_block_t*)SMQ_ADDRESS_OF(inst, root->next);

        //  找第一个子块
        *next = last->next;
        return SMQ_OK;
    }

    smq_block_t* cur = (smq_block_t*)SMQ_ADDRESS_OF(inst, *next);
    *next = cur->next;
    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_data(smq_inst inst, smq_msg msg, smq_void** data, smq_uint32* len, smq_uint32* cap)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;
    smq_block_t* block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    smq_uint32 count = 0;
    if (SMQ_NULL != data)
    {
        *data = block->data;
        count++;
    }
    
    if (SMQ_NULL != len)
    {
        *len  = block->data_size;
        count++;
    }
    
    if (SMQ_NULL != cap)
    {
        *cap  = smq->alloc_queues[block->queue_index]->block_size - sizeof(smq_block_t);
        count++;
    }

    SMQ_ASSERT((count > 0), "data、len、cap 三个参数均为 SMQ_NULL，这种用法是错误的，通常可能是代码存在什么 bug");

    return;
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

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_post(smq_inst inst, smq_msg msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;

    smq_mssge_queue_t* queue = smq->send_queue;
    if (SMQ_NULL == queue)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_POST;
    }

    register volatile smq_uint32 r    = queue->index_reader;
    register volatile smq_uint32 w    = queue->index_writer;
    register volatile smq_uint32 size = queue->size; 


    //  消息队列已满
    if (SMQ_QUEUE_FULL(size, r, w))
    {
        return SMQ_ERR_MSSGE_QUEUE_FULL;
    }

    //  将消息写入队列，并移动写点
    queue->messages[w]  = msg;
    queue->index_writer = ((w + 1) % size);

    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_wait(smq_inst inst, smq_int32 timeout, smq_msg* msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_NULL      != msg),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;

    smq_mssge_queue_t* queue = smq->recv_queue;
    if (SMQ_NULL == queue)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_WAIT;
    }

    register volatile smq_uint32 r = queue->index_reader;
    register volatile smq_uint32 w = queue->index_writer;
    register volatile smq_uint32 size = queue->size; 

    //  如果队列空
    if (SMQ_QUEUE_EMPTY(size, r, w))
    {
        //  下面开始等待
    }

    //  取出消息，修改reader
    *msg = queue->messages[r];
    queue->index_reader = ((r + 1) % size);

    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_peek(smq_inst inst, smq_uint32* count)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_NULL      != count),  "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;

    smq_mssge_queue_t* queue = smq->recv_queue;
    if (SMQ_NULL == queue)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_PEEK;
    }


    register volatile smq_uint32 r    = queue->index_reader;
    register volatile smq_uint32 w    = queue->index_writer;
    register volatile smq_uint32 size = queue->size; 

    *count = (size + (w - r)) % size;

    return SMQ_OK;
}


inline smq_void   SMQ_CALL    smq_msg_fill(smq_t* smq, smq_msg msg, smq_uint8** data, smq_uint32* len)
{
    smq_uint8* filled     = SMQ_NULL;
    smq_uint32 filled_len = 0;
    smq_uint32 cap  = 0;
    smq_msg_data(smq, msg, (smq_void**)&filled, &filled_len, &cap);
    smq_uint32 cache_remain_len = (smq_uint32)(cap - filled_len);

    smq_uint32 copy_len = smq_min(cache_remain_len, *len);
    smq_memcpy(filled + cache_remain_len, *data, copy_len);

    data += copy_len;
    *len -= copy_len;
}


SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_write(smq_inst inst, smq_msg msg, smq_void* data, smq_uint32 len)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_NULL      != data), "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;

    smq_block_t* root = (smq_block_t*)SMQ_ADDRESS_OF(smq, msg);
    smq_msg target = msg;
    if (SMQ_MSG_NULL != root->next)
    {
        target = root->next;    //  最后一个内存块
    }

    //  先填充最后一块
    smq_msg_fill(smq, target, (smq_uint8**)&data, &len);

    //  如果数据还未全部写入，那么分配个数据块继续写，写完后将数据块再合并到原消息上去
    while (len > 0)
    {
        smq_msg sub = SMQ_MSG_NULL;
        smq_errno err = smq_msg_new(inst, len, &sub);
        if (SMQ_OK != err)
        {
            return err;
        }

        smq_msg_fill(smq, sub, (smq_uint8** )&data, &len);
        smq_msg_merge(smq, msg, sub);
    }
    
    return SMQ_OK;
}



SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_read(smq_inst inst, smq_msg msg, smq_msg* itr, smq_void** data, smq_uint32* len)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_NULL      != itr),  "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_NULL      != data), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((SMQ_NULL      != len),  "关键输入参数，由外部保证有效性");

    smq_block_t* root = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    //  已经到达消息末尾，无法继续读取
    if (SMQ_MSG_NULL == *itr)
    {
        return SMQ_ERR_END_OF_MSG_CAN_NOT_READ;
    }

    //  获取消息数据
    smq_msg_data(inst, msg, data, len, SMQ_NULL);

    //  如果是从消息首部开始读取
    if (msg == *itr)
    {
        //  调整迭代指针
        if (SMQ_MSG_NULL == root->next)
        {
            //  没有子消息，提前结束
            *itr = SMQ_MSG_NULL;
            return SMQ_OK;
        } 

        //  有子消息，从第一个消息开始
        smq_block_t* last = (smq_block_t*)SMQ_ADDRESS_OF(inst, root->next);
        *itr = last->next;
        return SMQ_OK;
    }

    //  如果是最后一个消息
    if (root->next == *itr)
    {
        *itr = SMQ_MSG_NULL;
        return SMQ_OK;
    }

    *itr = root->next;
    return SMQ_OK;
}


