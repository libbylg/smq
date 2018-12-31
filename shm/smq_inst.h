#ifndef __smq_inst_H_
#define __smq_inst_H_

#include "smq.h"
#include "smq_limit.h"
#include "smq_os.h"




/// 这里定义了所有的可选项
///@{
#define SMQ_OPTION_BYTE_ENDIAN                  (0x0001)
///@}




#include "smq_alignpush.h"
#if   defined(_MSC_VER)
#pragma warning(disable:4200)   ///<    VC编译器会报告警：使用了非标准扩展 : 结构/联合中的零大小数组
#endif
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
    smq_uint32  next;           /// 下一个内存块
    smq_uint8   queue_index;    /// 内存块属于哪个队列
    smq_uint8   options;        /// 选项字段（当前必须填写为0）
    smq_uint16  data_size;      /// 用户数据大小
    smq_uint8   data[0];
}smq_block_t;


typedef struct 
{
    smq_uint32  block_size;
    smq_uint32  block_count;
    smq_uint32  block_last;
}smq_alloc_queue_t;


typedef struct
{
    smq_uint32  size;
    smq_uint32  index_reader;   //< index_reader指向的数据已读取
    smq_uint32  index_writer;   //< index_writer指向的数据还未写
    smq_uint32  messages[0];
}smq_mssge_queue_t;
#define SMQ_QUEUE_FULL(size,r,w)    (((w)<(r))?(((w) + 1) == (r)):((r)==0)&&((w)==((size)-1)))
#define SMQ_QUEUE_EMPTY(size,r,w)   ((r)==(w))
#if   defined(_MSC_VER)
#pragma warning(default:4200)   ///<    VC编译器会报告警：使用了非标准扩展 : 结构/联合中的零大小数组
#endif
#include "smq_alignpop.h"




/// smq实例对象，该对象的所有成员都支持0或者NULL作为初始值
typedef struct  
{
    smq_shm_t               shm;
    smq_uint32              role;
    smq_entry_t*            entry;
    smq_desc_t*             desc;
    smq_alloc_queue_t**     alloc_queues;
    smq_mssge_queue_t**     mssge_queues;
    smq_mssge_queue_t*      send_queue;
    smq_mssge_queue_t*      recv_queue;
    smq_uint8*              heap_data;
    smq_tid_t               recv_tid;
}smq_t;


#define SMQ_ADDRESS_OF(inst,offset)     ((smq_uint8*)(((smq_t*)(inst))->shm.addr) + (offset))
#define SMQ_OFFSETS_OF(inst,ptr)        ((smq_uint32)((smq_uint8*)(ptr) - (smq_uint8*)(((smq_t*)(inst))->shm.addr)))


#endif//__smq_inst_H_
