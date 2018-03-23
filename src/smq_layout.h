#ifndef __smq_layout_H_
#define __smq_layout_H_
#include "smq_alignpush.h"

typedef struct  
{
    smq_uint16  check_sum;              ///<    ����entry��У���
    smq_uint16  version;                ///<    �����ڴ沼�ְ汾��
    
    smq_uint16  options;                ///<    ѡ��
    smq_uint8   alloc_queues_count;     ///<    ���ٸ��������
    smq_uint8   mssge_queues_count;     ///<    ���ٸ���Ϣ����
    
    smq_uint32  mark;                   ///<    ����ֶΣ�����entry��������������ͬ�ģ�������ֶ���������ÿ�����´����Ĺ����ڴ�
    smq_uint32  heap_len;               ///<    �����ѵĳ���
    smq_uint32  heap_data;              ///<    �ѵ���ʼ��ַ��ͬʱҲ�ǹ����ڴ�ͷ���ĳ���

    smq_uint8   padding[108];           ///<    ����ֶΣ����ڲ���128�ֽ�
}smq_entry_t;

typedef struct  
{
    smq_uint32  len;
    smq_char    name[SMQ_FULL_MAPPING_NAME_LEN_MAX];    ///<    �����ڴ��ȫ��
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
