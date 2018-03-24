#ifndef __smq_inst_H_
#define __smq_inst_H_

#include "smq_os.h"
#include "smq_layout.h"
#include "smq_limit.h"

/// smqʵ�����󣬸ö�������г�Ա��֧��0����NULL��Ϊ��ʼֵ
typedef struct  
{
    smq_shm_t               shm;
    smq_entry_t*            entry;
    smq_alloc_queue_t**     alloc_groups;
    smq_message_queue_t**   message_group;
    smq_uint8*              heap_data;
}smq_t;

#endif