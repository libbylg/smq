#ifndef __smq_layout_H_
#define __smq_layout_H_

#include "smq.h"
#include "smq_limit.h"



/// 这里定义了所有的可选项
///@{
#define SMQ_OPTION_BYTE_ENDIAN                  (0x0001)
///@}




#if defined(WIN32) || defined(WIN64)
#pragma warning(disable:4200)   ///<    VC编译器会报告警：使用了非标准扩展 : 结构/联合中的零大小数组
#endif

#include "smq_alignpush.h"
// #if defined(WIN32) || defined(WIN64)
// #pragma pack(1)
// #endif


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
    smq_uint32  idle_block_last;
}smq_alloc_queue_t;


typedef struct
{
    smq_uint32  size;
    smq_uint32  index_reader;
    smq_uint32  index_writer;
    smq_uint32  messages[0];
}smq_mssge_queue_t;


#include "smq_alignpop.h"
// #if defined(WIN32) || defined(WIN64)
// #pragma pack(pop)
// #endif

#if defined(WIN32) || defined(WIN64)
#pragma warning(default:4200)   ///<    VC编译器会报告警：使用了非标准扩展 : 结构/联合中的零大小数组
#endif

#endif//__smq_layout_H_
