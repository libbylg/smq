#ifndef __os_H_
#define __os_H_

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "smq.h"
#include "smq_limit.h"

#define SMQ_BYTE_ENDIAN     SMQ_BYTE_ENDIAN_SMALL

#define SMQ_TLS __declspec(thread)


#define SMQ_BOOL    BOOL
#define SMQ_TRUE    (1)
#define SMQ_FALSE   (0)

#define SMQ_MAPPING_PREFIX  "Global\\"

typedef struct
{
    /// 下面为所有OS都应该具备的字段
    smq_uint8*  addr;       ///<    共享内存映射到本进程的地址
    smq_uint32  real_size;  ///<    共享内存的真实大小
    SMQ_BOOL    is_exist;   ///<    是否打开的已经存在的对象
    smq_char    base_name[SMQ_MAPPING_NAME_LEN_MAX];        ///<    共享内存对象的基本名称
    smq_char    full_name[SMQ_FULL_MAPPING_NAME_LEN_MAX];   ///<    共享内存对象的全名
    HANDLE      handle;     ///<    共享内存对象的内核句柄
    smq_uint32  os_error;
}smq_shm_t;

SMQ_EXTERN  smq_errno   smq_shm_open(smq_char* name, smq_uint32 name_len, smq_uint32 size,  SMQ_BOOL writable, smq_shm_t* shm);
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


SMQ_EXTERN  smq_uint32  smq_get_pid();


#define     SMQ_INVALID_TID (0)
typedef     DWORD       smq_tid_t;
SMQ_EXTERN  smq_tid_t   smq_get_tid();

#endif//__os_H_
