#include "smq_inst.h"

#include "smq_libc.h"
#include "smq_errors.h"
#include "smq_params.h"
#include "smq_asserts.h"
#include "smq_utils.h"
#include "smq_logs.h"




static smq_uint8*   smq_cut(smq_uint8** pos, smq_uint32 size)
{
    smq_uint8* ret = *pos;
    *pos += size;
    return ret;
}



static smq_void smq_layout_map(smq_t* smq)
{
    smq_uint8*  pos     =   smq->shm.addr;
    smq->entry          =   (smq_entry_t*)smq_cut(&pos, sizeof(smq_entry_t));
    smq->desc           =   (smq_desc_t*)smq_cut(&pos, sizeof(smq_desc_t));

    for (smq_uint32 i = 0; i < smq->entry->alloc_queues_count; i++)
    {
        smq->alloc_queues[i]    =   (smq_alloc_queue_t*)smq_cut(&pos, sizeof(smq_alloc_queue_t));
    }

    for (smq_uint32 i = 0; i < smq->entry->mssge_queues_count; i++)
    {
        smq->mssge_queues[i]    =   (smq_mssge_queue_t*)smq_cut(&pos, sizeof(smq_mssge_queue_t));
    }

    smq->heap_data  =   SMQ_ADDRESS_OF(smq, smq->entry->heap_data);
}




static smq_void smq_layout_alloc_queues_init(smq_t* smq)
{
    smq_uint32  each_size   =   smq->entry->heap_len;

    smq_uint8* pos = smq->heap_data;
    for (smq_uint32 q = 0; q < smq->entry->alloc_queues_count; q++)
    {
        smq_alloc_queue_t* queue = (smq_alloc_queue_t*)(smq->alloc_queues[q]);
        queue->block_size   =   SMQ_MEMORY_BLOCK_SIZE_MAX >> (smq->entry->alloc_queues_count - (q + 1));
        queue->block_count  =   each_size / queue->block_size;
        
        //  最后一个队列需要为Header预留一部分存储区
        if ((q + 1) == smq->entry->alloc_queues_count)
        {
            SMQ_ASSERT((each_size >= smq->entry->heap_data), "最后一个队列的总内存必须大于，HeapHeader的长度（smq->entry->heap_data就是HeapHeader的长度）")
            queue->block_count = (each_size - smq->entry->heap_data) / queue->block_size;
        }

        //  下面开始构造队列中的所有内存块
        smq_block_t* first = (smq_block_t*)smq_cut(&pos, queue->block_size);
        first->queue_index = q;
        first->options = 0;
        smq_block_t* last = first;
        for (smq_uint32 index = 1; index < queue->block_count; index++)
        {
            smq_block_t* block = (smq_block_t*)smq_cut(&pos, queue->block_size);
            block->queue_index = q;
            block->options = 0;
            block->next = 0;
            last->next = SMQ_OFFSETS_OF(smq, block);
            last = block;
        }

        //  队列的指针总是指向最后一个内存块，而最后一块内存总是指向第一块内存
        //  这样形成一个环状，无论是向对位添加内存块还是从队首获取内存块都非常方便
        queue->idle_block_last = SMQ_OFFSETS_OF(smq, last);
        last->next = SMQ_OFFSETS_OF(smq, first);
    }
}




static smq_void smq_layout_init(smq_t* smq)
{
    smq_uint8*  pos     =   smq->shm.addr;

    //  初始化entry
    smq->entry          =   (smq_entry_t*)smq_cut(&pos, sizeof(smq_entry_t));
    smq_memset(smq->entry, 0, sizeof(smq_entry_t));
    smq->entry->check_sum           =   0;
    smq->entry->version             =   SMQ_VERSION_MAX;
    smq->entry->options             =   ((SMQ_BYTE_ENDIAN_BIG == SMQ_BYTE_ENDIAN)?SMQ_OPTION_BYTE_ENDIAN:0x0000);
    smq->entry->alloc_queues_count  =   SMQ_ALLOC_QUEUES_COUNT;
    smq->entry->mssge_queues_count  =   SMQ_MSSGE_QUEUES_COUNT;
    smq->entry->mark                =   smq_proc_getpid();
    smq->entry->heap_len            =   smq->shm.real_size;

    //  初始化desc
    smq->desc           =   (smq_desc_t*)smq_cut(&pos, sizeof(smq_desc_t));
    smq_memset(smq->desc, 0, sizeof(smq_desc_t));
    smq->desc->len  =   sizeof(smq_desc_t);
    smq_memcpy(smq->desc->name, smq->shm.full_name, smq_strlen(smq->shm.full_name) + 1);

    //  初始化分配队列
    for (smq_uint32 i = 0; i < smq->entry->alloc_queues_count; i++)
    {
        smq->alloc_queues[i]     =  (smq_alloc_queue_t*)smq_cut(&pos, sizeof(smq_alloc_queue_t));
    }

    //  初始化消息队列
    for (smq_uint32 i = 0; i < smq->entry->mssge_queues_count; i++)
    {
        smq_uint32 message_queue_size = sizeof(smq_mssge_queue_t) + sizeof(smq_uint32) * smq_params.queue_size;
        smq->mssge_queues[i]        =   (smq_mssge_queue_t*)smq_cut(&pos, message_queue_size);
        smq->mssge_queues[i]->size  =   message_queue_size;
        smq->mssge_queues[i]->index_reader  =   0;
        smq->mssge_queues[i]->index_writer  =   0;
    }

    //  重新定位heap_data，重新统一到4096字节的边界
    smq->entry->heap_data   =   SMQ_OFFSETS_OF(smq, pos);
    if (0 != (smq->entry->heap_data % 4096))
    {
        smq->entry->heap_data   =   4096 * ((smq->entry->heap_data / 4096) + 1);
        smq->heap_data          =   SMQ_ADDRESS_OF(smq, smq->entry->heap_data);
    }

    //  entry的所有字段已经填充完毕，此时重新计算entry的checksum
    smq->entry->check_sum   =   smq_checksum(smq->shm.addr, sizeof(smq_entry_t));

    //  下面开始对共享内存进行存储划分
    smq_layout_alloc_queues_init(smq);

    //  根据角色的不同，确定收发队列为哪个
    switch (smq->role)
    {
    case SMQ_ROLE_LEADER:
        smq->recv_queue = smq->mssge_queues[0];
        smq->send_queue = smq->mssge_queues[1];
        break;
    case SMQ_ROLE_FOLLOWER:
        smq->recv_queue = smq->mssge_queues[1];
        smq->send_queue = smq->mssge_queues[0];
        break;
    default:
        smq->recv_queue = NULL;
        smq->send_queue = NULL;
        break;
    }
}




static smq_errno   SMQ_CALL    smq_layout_load(smq_t* smq)
{
    //  创建进程间互斥锁
    smq_proc_mutex_t mutex = {0};
    smq_errno err = smq_proc_mutex_open(smq->shm.base_name, &mutex);
    if (SMQ_OK != err)
    {
        return err;
    }

    //  执行加锁操作
    err = smq_proc_mutex_lock(&mutex);
    if (SMQ_OK != err)
    {
        return err;
    }

    //  通过校验和确定一下共享内存是否完成了初始化
    if (0 == smq_checksum(smq->shm.addr, sizeof(smq_entry_t)))
    {
        smq_layout_map(smq);
    }
    else
    {
        smq_layout_init(smq);
    }

    //  初始化完成执行解锁操作
    err = smq_proc_mutex_unlock(&mutex);
    if (SMQ_OK != err)
    {
        SMQ_WARN(SMQ_LOG_INIT_UNLOCK_FAILED, "errno=%d, mutex-name=%s", err, mutex.full_name);
    }

    //  释放进程级锁
    smq_proc_mutex_close(&mutex);

    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_open(smq_char* name, smq_uint32 role, smq_inst* inst)
{
    SMQ_ASSERT((NULL != name), "名字不能为空");
    SMQ_ASSERT((NULL != inst), "inst 不能为空");

    if ((role < SMQ_ROLE_MIN) || (role > SMQ_ROLE_MAX))
    {
        return SMQ_ERR_UNSUPORTED_ROLE;
    }


    //  创建smq实例
    smq_t* smq = (smq_t*)malloc(sizeof(smq_t));
    if (NULL == smq)
    {
        return SMQ_ERR_MALLOC_FAILED;
    }
    smq_memset(smq, 0, sizeof(smq_t));
    smq->role = role;


    //  创建共享内存对象
    int shm_size = smq_params.memory_size * 1024 * 1024;
    smq_errno err = smq_shm_open(name, shm_size, &(smq->shm));
    if (SMQ_OK != err)
    {
        smq_free(smq);
        return SMQ_ERR_INIT_OPEN_OR_CREATE_SHM_FAILED;
    }


    //  初始化共享内存布局
    err = smq_layout_load(smq);
    if (SMQ_OK != err)
    {
        smq_shm_close(&(smq->shm));

        if (NULL == smq->alloc_queues)
        {
            smq_free(smq->alloc_queues);
        }

        if (NULL == smq->mssge_queues)
        {
            smq_free(smq->mssge_queues);
        }

        smq_free(smq);
        return SMQ_ERR_INIT_LAYOUT_FAILED;
    }

    *inst = smq;

    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_void    SMQ_CALL    smq_close(smq_inst inst)
{
    if (NULL == inst)
    {
        return;
    }

    smq_t* smq = (smq_t*)inst;
    smq_shm_close(&(smq->shm));

    if (NULL != smq->alloc_queues)
    {
        smq_free(smq->alloc_queues);
    }

    if (NULL != smq->mssge_queues)
    {
        smq_free(smq->mssge_queues);
    }

    smq_free(smq);
}




SMQ_EXTERN  SMQ_API smq_errno   SMQ_CALL    smq_version(smq_inst inst, smq_uint32* ver)
{
    SMQ_ASSERT((NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((NULL != ver), "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;

    *ver = smq->entry->version;

    return SMQ_OK;
}




SMQ_EXTERN  SMQ_API smq_void   SMQ_CALL    smq_dump(smq_inst inst, smq_uint32 range, smq_void* context, SMQ_DUMPER_FUNC f)
{
    SMQ_ASSERT((NULL != inst), "关键输入参数，由外部保证有效性");
    SMQ_ASSERT((NULL != f),    "关键输入参数，由外部保证有效性");

    smq_t* smq = (smq_t*)inst;

    //  dump开始
    smq_uint32 flag = 0;
    if (0 != (*f)(context, flag++, NULL, 0))
    {
        return;
    }

    //  dump头部
    if (0 != (range & SMQ_DUMP_RANGE_HEAP_HEAD))
    {
        if (0 != (*f)(context, flag++, smq->shm.addr, smq->entry->heap_data))
        {
            return;
        }
    }

    //  dump数据
    if (0 != (range & SMQ_DUMP_RANGE_HEAP_DATA))
    {
        if (0 != (*f)(context, flag++, smq->shm.addr, smq->entry->heap_data))
        {
            return;
        }
    }

    //  dump结束
    if (0 != (*f)(context, (smq_uint32)(~0), NULL, 0))
    {
        return;
    }

    return;
}








