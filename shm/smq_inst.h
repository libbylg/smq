#ifndef __smq_inst_H_
#define __smq_inst_H_

#include "smq.h"
#include "smq_limit.h"
#include "smq_os.h"




/// ���ﶨ�������еĿ�ѡ��
///@{
#define SMQ_OPTION_BYTE_ENDIAN                  (0x0001)
///@}




#include "smq_alignpush.h"
#if   defined(_MSC_VER)
#pragma warning(disable:4200)   ///<    VC�������ᱨ�澯��ʹ���˷Ǳ�׼��չ : �ṹ/�����е����С����
#endif
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
    smq_uint32  block_last;
}smq_alloc_queue_t;


typedef struct
{
    smq_uint32  size;
    smq_uint32  index_reader;   //< index_readerָ��������Ѷ�ȡ
    smq_uint32  index_writer;   //< index_writerָ������ݻ�δд
    smq_uint32  messages[0];
}smq_mssge_queue_t;
#define SMQ_QUEUE_FULL(size,r,w)    (((w)<(r))?(((w) + 1) == (r)):((r)==0)&&((w)==((size)-1)))
#define SMQ_QUEUE_EMPTY(size,r,w)   ((r)==(w))
#if   defined(_MSC_VER)
#pragma warning(default:4200)   ///<    VC�������ᱨ�澯��ʹ���˷Ǳ�׼��չ : �ṹ/�����е����С����
#endif
#include "smq_alignpop.h"




/// smqʵ�����󣬸ö�������г�Ա��֧��0����NULL��Ϊ��ʼֵ
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
