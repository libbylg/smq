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



static  smq_msg  smq_queue_pop_front(smq_t* smq, smq_alloc_queue_t* queue)
{
    if (SMQ_MSG_NULL == queue->idle_block_last)
    {
        return SMQ_MSG_NULL;
    }

    smq_msg msg = SMQ_MSG_NULL;

    //  ������в�Ϊ��
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

    //  �ҵ����������Ķ���
    smq_alloc_queue_t* queue = (smq_alloc_queue_t*)smq->alloc_queues[cur_block->queue_index];

    //  �ҵ������е����һ���飬Ȼ�󽫵�ǰҪ���յĿ�������ȥ
    smq_block_t* queue_last = (smq_block_t*)SMQ_ADDRESS_OF(smq, queue->idle_block_last);
    cur_block->next  = queue_last->next;
    queue_last->next = msg;

    //  �޸ĵ�ǰ���е����һ�����ָ��
    queue->idle_block_last = msg;
    return SMQ_OK;
}




/// �ƽ������������Ƿ����ƽ�֮ǰ����Ϣ
static smq_msg smq_itr_advance(smq_t* smq, smq_msg* itr)
{
    smq_msg ret = *itr;
    *itr = ((smq_block_t*)SMQ_ADDRESS_OF(smq, *itr))->next;
    return ret;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_new(smq_inst inst, smq_uint32 size, smq_msg* msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_NEW;
    }

    smq_int32 primary_index = smq_alloc_queue_index(smq, size);
    for (smq_int32 i = primary_index; i >= 0; i--)
    {
        //  �������Ϊ��
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
        //  �������Ϊ��
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




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_del(smq_inst inst, smq_msg msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_DEL;
    }

    //  �������Ϣ������Ϣ����ô��Ҫ��ɾ������Ϣ
    smq_block_t* block = (smq_block_t*)SMQ_ADDRESS_OF(smq, msg);
    if (SMQ_MSG_NULL != block->next)
    {
        //  �������Ϣ������Ϣ����ô�ӵ�һ����Ϣ��ʼɾ�������Ϣ��last->next Ϊ��һ������Ϣ
        smq_block_t* last  = (smq_block_t*)SMQ_ADDRESS_OF(smq, block->next);
        for (smq_msg pos = last->next; pos != block->next; )
        {
            smq_queue_push_back(smq, smq_itr_advance(smq, &pos));
        }

        //  ����ɾ�����һ������Ϣ
        smq_queue_push_back(smq, block->next);
    }

    //  ���ɾ�� msg ����
    smq_queue_push_back(smq, msg);

    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_merge(smq_inst inst, smq_msg msg, smq_msg sub)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != sub),  "�ؼ�������������ⲿ��֤��Ч��");

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_MERGE;
    }

    smq_block_t* cur_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);
    smq_block_t* sub_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub);
    
    //  ��û������Ϣ
    if (SMQ_MSG_NULL == cur_block->next)
    {
        cur_block->next = sub;
        sub_block->next = sub;
        return SMQ_OK;
    }

    //  ���¹�������Ϣ��
    smq_block_t* last = (smq_block_t*)SMQ_ADDRESS_OF(inst, cur_block->next);
    sub_block->next = last->next;
    last->next      = sub;

    //  ���ö�βָ��
    cur_block->next     = sub;

    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((NULL          != next), "�ؼ�������������ⲿ��֤��Ч��");

    smq_t* smq = (smq_t*)inst;
    smq_block_t* cur_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    //  ���msgû����Ԫ�ػ����Ѿ�������Ϣ����β����ע���������������жϷ�ʽ��һģһ���ģ�
    if (cur_block->next == *next)
    {
        *next = SMQ_MSG_NULL;
        return  SMQ_OK;
    }

    //  ����ǵ�һ�ε���
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
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");

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

    SMQ_ASSERT((count > 0), "data��len��cap ����������Ϊ NULL�������÷��Ǵ���ģ�ͨ�������Ǵ������ʲô bug");

    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_fix(smq_inst inst, smq_msg msg, smq_uint32 len)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");

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
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");

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


