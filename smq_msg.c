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

    //  ������в�Ϊ��
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

    //  �ҵ����������Ķ���
    smq_alloc_queue_t* queue = (smq_alloc_queue_t*)smq->alloc_queues[cur_block->queue_index];

    //  �ҵ������е����һ���飬Ȼ�󽫵�ǰҪ���յĿ�������ȥ
    smq_block_t* queue_last = (smq_block_t*)SMQ_ADDRESS_OF(smq, queue->block_last);
    cur_block->next  = queue_last->next;
    queue_last->next = msg;

    //  �޸ĵ�ǰ���е����һ�����ָ��
    queue->block_last = msg;
    return SMQ_OK;
}




/// �ƽ������������Ƿ����ƽ�֮ǰ����Ϣ
inline smq_msg smq_itr_advance(smq_t* smq, smq_msg* itr)
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




SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_del(smq_inst inst, smq_msg msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");

    smq_t* smq = (smq_t*)inst;
    if (SMQ_ROLE_VIEWER == smq->role)
    {
        SMQ_WARN(SMQ_LOG_READONLY_INST_UNSUPPORT_MSG_DEL, "msg=0x%x", msg);
        return;
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

    return;
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

    smq_block_t* msg_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);
    smq_block_t* sub_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub);
    
    //  ��û������Ϣ
    if (SMQ_MSG_NULL == msg_block->next)
    {
        if (SMQ_MSG_NULL == sub_block->next)
        {
            msg_block->next = sub;  ///<    ���޸�msg��ָ�룺sub���������ӵ�ָ��

            //  ��������Ϣ��
            sub_block->next = sub;
        }
        else
        {
            msg_block->next = sub_block->next;  ///<    ���޸�msg��ָ�룺sub_block->next ������Ϣ�������һ����Ϣ

            //  ��������Ϣ��
            smq_block_t* sub_last_block  = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub_block->next);
            sub_block->next = sub_last_block->next;
            sub_last_block->next = sub;     ///<    sub������Ϊmsg�ĵ�һ����Ϣ
        }
        
        return SMQ_OK;
    }

    //  ƴ������Ϣ��
    smq_block_t* msg_last_block = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg_block->next);
    if (SMQ_MSG_NULL == sub_block->next)
    {
        msg_block->next = sub;  ///<    ���޸�msg��ָ�룺sub���������ӵ�ָ��

        //  ��������Ϣ��
        sub_block->next = msg_last_block->next;
        msg_last_block->next = sub;
    }
    else
    {
        msg_block->next = sub_block->next;  ///<    ���޸�msg��ָ�룺sub_block->next ������Ϣ�������һ����Ϣ

        //  ��������Ϣ��
        smq_block_t* sub_last_block  = (smq_block_t*)SMQ_ADDRESS_OF(inst, sub_block->next);
        sub_block->next = sub_last_block->next;
        sub_last_block->next = msg_last_block->next;
    }
    
    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_next(smq_inst inst, smq_msg msg, smq_msg* next)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_NULL      != next), "�ؼ�������������ⲿ��֤��Ч��");

    smq_t* smq = (smq_t*)inst;
    smq_block_t* root = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    //  ����Ѿ��ﵽ���һ����Ϣ
    if (SMQ_MSG_NULL == *next)
    {
        return SMQ_ERR_END_OF_MSG_NO_NEXT;
    }

    //  ��������һ����Ϣ����һ����Ϣ
    if (root->next == *next)
    {
        *next = SMQ_MSG_NULL;
        return SMQ_OK;
    }

    //  ���nextָ��msg����ζ���û�ʵ�����ȡmsg�ĵ�һ������Ϣ
    if (msg == *next)
    {
        //  ���û������Ϣ
        if (SMQ_MSG_NULL == root->next)
        {
            *next = SMQ_MSG_NULL;
            return SMQ_OK;
        }

        smq_block_t* last = (smq_block_t*)SMQ_ADDRESS_OF(inst, root->next);

        //  �ҵ�һ���ӿ�
        *next = last->next;
        return SMQ_OK;
    }

    smq_block_t* cur = (smq_block_t*)SMQ_ADDRESS_OF(inst, *next);
    *next = cur->next;
    return  SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_msg_data(smq_inst inst, smq_msg msg, smq_void** data, smq_uint32* len, smq_uint32* cap)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");

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

    SMQ_ASSERT((count > 0), "data��len��cap ����������Ϊ SMQ_NULL�������÷��Ǵ���ģ�ͨ�������Ǵ������ʲô bug");

    return;
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

SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_post(smq_inst inst, smq_msg msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");

    smq_t* smq = (smq_t*)inst;

    smq_mssge_queue_t* queue = smq->send_queue;
    if (SMQ_NULL == queue)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_POST;
    }

    register volatile smq_uint32 r    = queue->index_reader;
    register volatile smq_uint32 w    = queue->index_writer;
    register volatile smq_uint32 size = queue->size; 


    //  ��Ϣ��������
    if (SMQ_QUEUE_FULL(size, r, w))
    {
        return SMQ_ERR_MSSGE_QUEUE_FULL;
    }

    //  ����Ϣд����У����ƶ�д��
    queue->messages[w]  = msg;
    queue->index_writer = ((w + 1) % size);

    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_wait(smq_inst inst, smq_int32 timeout, smq_msg* msg)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_NULL      != msg),  "�ؼ�������������ⲿ��֤��Ч��");

    smq_t* smq = (smq_t*)inst;

    smq_mssge_queue_t* queue = smq->recv_queue;
    if (SMQ_NULL == queue)
    {
        return SMQ_ERR_READONLY_INST_UNSUPPORT_MSG_WAIT;
    }

    register volatile smq_uint32 r = queue->index_reader;
    register volatile smq_uint32 w = queue->index_writer;
    register volatile smq_uint32 size = queue->size; 

    //  ������п�
    if (SMQ_QUEUE_EMPTY(size, r, w))
    {
        //  ���濪ʼ�ȴ�
    }

    //  ȡ����Ϣ���޸�reader
    *msg = queue->messages[r];
    queue->index_reader = ((r + 1) % size);

    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_msg_peek(smq_inst inst, smq_uint32* count)
{
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_NULL      != count),  "�ؼ�������������ⲿ��֤��Ч��");

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
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_NULL      != data), "�ؼ�������������ⲿ��֤��Ч��");

    smq_t* smq = (smq_t*)inst;

    smq_block_t* root = (smq_block_t*)SMQ_ADDRESS_OF(smq, msg);
    smq_msg target = msg;
    if (SMQ_MSG_NULL != root->next)
    {
        target = root->next;    //  ���һ���ڴ��
    }

    //  ��������һ��
    smq_msg_fill(smq, target, (smq_uint8**)&data, &len);

    //  ������ݻ�δȫ��д�룬��ô��������ݿ����д��д������ݿ��ٺϲ���ԭ��Ϣ��ȥ
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
    SMQ_ASSERT((SMQ_INST_NULL != inst), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_MSG_NULL  != msg),  "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_NULL      != itr),  "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_NULL      != data), "�ؼ�������������ⲿ��֤��Ч��");
    SMQ_ASSERT((SMQ_NULL      != len),  "�ؼ�������������ⲿ��֤��Ч��");

    smq_block_t* root = (smq_block_t*)SMQ_ADDRESS_OF(inst, msg);

    //  �Ѿ�������Ϣĩβ���޷�������ȡ
    if (SMQ_MSG_NULL == *itr)
    {
        return SMQ_ERR_END_OF_MSG_CAN_NOT_READ;
    }

    //  ��ȡ��Ϣ����
    smq_msg_data(inst, msg, data, len, SMQ_NULL);

    //  ����Ǵ���Ϣ�ײ���ʼ��ȡ
    if (msg == *itr)
    {
        //  ��������ָ��
        if (SMQ_MSG_NULL == root->next)
        {
            //  û������Ϣ����ǰ����
            *itr = SMQ_MSG_NULL;
            return SMQ_OK;
        } 

        //  ������Ϣ���ӵ�һ����Ϣ��ʼ
        smq_block_t* last = (smq_block_t*)SMQ_ADDRESS_OF(inst, root->next);
        *itr = last->next;
        return SMQ_OK;
    }

    //  ��������һ����Ϣ
    if (root->next == *itr)
    {
        *itr = SMQ_MSG_NULL;
        return SMQ_OK;
    }

    *itr = root->next;
    return SMQ_OK;
}


