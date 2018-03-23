#ifndef __smq_layout_H_
#define __smq_layout_H_
#include "smq_alignpush.h"

typedef struct  
{
    smq_uint16  check_sum;              ///<    整个entry的校验和
    smq_uint16  version;                ///<    共享内存布局版本号
    
    smq_uint16  options;                ///<    选项
    smq_uint8   alloc_queues_count;     ///<    多少个分配队列
    smq_uint8   mssge_queues_count;     ///<    多少个消息队列
    
    smq_uint32  mark;                   ///<    标记字段，由于entry的内容往往是相同的，加入该字段用于区别每次重新创建的共享内存
    smq_uint32  heap_len;               ///<    整个堆的长度
    smq_uint32  heap_data;              ///<    堆的起始地址，同时也是共享内存头部的长度

    smq_uint8   padding[108];           ///<    填充字段，用于补齐128字节
}smq_entry_t;

typedef struct  
{
    smq_uint32  len;
    smq_char    name[SMQ_FULL_MAPPING_NAME_LEN_MAX];    ///<    共享内存的全名
}smq_desc_t;

typedef struct  
{
    smq_uint32  next;
    smq_uint8   queue_index8;
    smq_uint8   reversed;
}smq_memory_block_t;

typedef struct
{
    smq_uint16  data_size;
    smq_uint32  next;
    smq_uint8   data[0];
}smq_message_t;

typedef struct 
{
    smq_uint32  block_size;
    smq_uint32  block_count;
    smq_uint32  idle_queue_first;
    smq_uint32  idle_queue_last;
    smq_uint32  work_queue_first;
    smq_uint32  work_queue_last;
}smq_alloc_queue_t;

typedef struct
{
    smq_uint32  len;
    smq_uint32  index_reader;
    smq_uint32  index_writer;
}smq_message_queue_t;



#include "smq_alignpop.h"
#endif//__smq_layout_H_
