#ifndef __smq_inst_H_
#define __smq_inst_H_

#include "smq.h"
#include "smq_os.h"
#include "smq_layout.h"

/// smq实例对象，该对象的所有成员都支持0或者NULL作为初始值
typedef struct  
{
    smq_shm_t               shm;
    smq_entry_t*            entry;
    smq_desc_t*             desc;
    smq_alloc_queue_t**     alloc_queues;
    smq_mssge_queue_t**     mssge_groups;
    smq_uint8*              heap_data;
}smq_t;

#define SMQ_ADDRESS_OF(inst,offset)     ((smq_uint8*)(((smq_t*)(inst))->shm.addr) + (offset))
#define SMQ_OFFSETS_OF(inst,ptr)        ((smq_uint32)((smq_uint8*)(ptr) - (smq_uint8*)(((smq_t*)(inst))->shm.addr)))

#endif//__smq_inst_H_
