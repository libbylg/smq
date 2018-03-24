#ifndef __os_H_
#define __os_H_

#include "smq.h"

#include "smq_limit.h"

#include "windows.h"

#define SMQ_TLS __declspec(thread)

typedef struct
{
    /// ����Ϊ����OS��Ӧ�þ߱����ֶ�
    smq_uint8*  addr;       ///<    �����ڴ�ӳ�䵽�����̵ĵ�ַ
    smq_uint32  real_size;  ///<    �����ڴ����ʵ��С
    smq_uint32  is_exist;   ///<    �Ƿ�򿪵��Ѿ����ڵĶ���
    smq_char    full_name[SMQ_FULL_MAPPING_NAME_LEN_MAX];   ///<    �����ڴ�����ȫ��

    /// ����Ϊƽ̨�ض����ֶ�
    HANDLE      handle;     ///<    �����ڴ������ں˾��
}smq_shm_t;

SMQ_EXTERN  smq_errno   smq_shm_open(smq_char* name, smq_uint32 size, smq_shm_t* shm);
SMQ_EXTERN  smq_void    smq_shm_close(smq_shm_t* shm);



typedef struct
{
    HANDLE      handle;
    smq_char    full_name[256];
}smq_proc_mutex_t;

SMQ_EXTERN  smq_errno   smq_proc_mutex_open(smq_char* name, smq_proc_mutex_t* mutex);
SMQ_EXTERN  smq_errno   smq_proc_mutex_lock(smq_proc_mutex_t* mutex);
SMQ_EXTERN  smq_errno   smq_proc_mutex_unlock(smq_proc_mutex_t* mutex);
SMQ_EXTERN  smq_void    smq_proc_mutex_close(smq_proc_mutex_t* mutex);

#endif//__os_H_
