#ifndef __smq_layout_H_
#define __smq_layout_H_

#include "smq.h"
#include "smq_limit.h"



/// ���ﶨ�������еĿ�ѡ��
///@{
#define SMQ_OPTION_BYTE_ENDIAN                  (0x0001)
///@}




#if defined(WIN32) || defined(WIN64)
#pragma warning(disable:4200)   ///<    VC�������ᱨ�澯��ʹ���˷Ǳ�׼��չ : �ṹ/�����е����С����
#endif

#include "smq_alignpush.h"
// #if defined(WIN32) || defined(WIN64)
// #pragma pack(1)
// #endif


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
    smq_uint32  next;           /// ��һ���ڴ��
    smq_uint8   queue_index;    /// �ڴ�������ĸ�����
    smq_uint8   options;        /// ѡ���ֶΣ���ǰ������дΪ0��
    smq_uint16  data_size;      /// �û����ݴ�С
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
#pragma warning(default:4200)   ///<    VC�������ᱨ�澯��ʹ���˷Ǳ�׼��չ : �ṹ/�����е����С����
#endif

#endif//__smq_layout_H_
