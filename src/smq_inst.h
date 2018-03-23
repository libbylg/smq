#ifndef __smq_inst_H_
#define __smq_inst_H_

#include "smq_os.h"
#include "smq_layout.h"
#include "smq_limit.h"


typedef struct  
{
    smq_shm_t               shm;
    smq_entry_t*            entry;
    smq_alloc_queue_t*      alloc_groups[SMQ_ALLOC_QUEUES_COUNT];
    smq_message_queue_t*    message_group[SMQ_MSSGE_QUEUES_COUNT];
    smq_uint8*              heap_data;
}smq_t;

#endif