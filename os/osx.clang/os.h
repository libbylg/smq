//
//  os.h
//  smq
//
//  Created by luolijun on 2018/4/1.
//  Copyright 漏 2018骞� luolijun. All rights reserved.
//

#ifndef os_h
#define os_h


#include "smq.h"
#include "smq_limit.h"


#define SMQ_BYTE_ENDIAN     SMQ_BYTE_ENDIAN_SMALL

#define SMQ_TLS             __declspec(thread)


#define SMQ_BOOL            BOOL
#define SMQ_TRUE            (1)
#define SMQ_FALSE           (0)

#define SMQ_MAPPING_PREFIX  "Global\\"

typedef struct
{
    /// 锟斤拷锟斤拷为锟斤拷锟斤拷OS锟斤拷应锟矫具憋拷锟斤拷锟街讹拷
    smq_uint8*  addr;       ///<    锟斤拷锟斤拷锟节达拷映锟戒到锟斤拷锟斤拷锟教的碉拷址
    smq_uint32  real_size;  ///<    锟斤拷锟斤拷锟节达拷锟斤拷锟斤拷实锟斤拷小
    SMQ_BOOL    is_exist;   ///<    锟角凤拷锟津开碉拷锟窖撅拷锟斤拷锟节的讹拷锟斤拷
    smq_char    base_name[SMQ_MAPPING_NAME_LEN_MAX];        ///<    锟斤拷锟斤拷锟节达拷锟斤拷锟斤拷锟侥伙拷锟斤拷锟斤拷锟斤拷
    smq_char    full_name[SMQ_FULL_MAPPING_NAME_LEN_MAX];   ///<    锟斤拷锟斤拷锟节达拷锟斤拷锟斤拷锟斤拷全锟斤拷
    HANDLE      handle;     ///<    锟斤拷锟斤拷锟节达拷锟斤拷锟斤拷锟斤拷锟节核撅拷锟斤拷
    smq_uint32  os_error;
}smq_shm_t;

SMQ_EXTERN  smq_errno   smq_shm_open(smq_char* name, smq_uint32 name_len, smq_uint32 size,  SMQ_BOOL writable, smq_shm_t* shm);
SMQ_EXTERN  smq_void    smq_shm_close(smq_shm_t* shm);



typedef struct
{
    //HANDLE      handle;
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



#endif /* os_h */
