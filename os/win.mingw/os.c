#include "smq_os.h"


#include "smq_asserts.h"


BOOL APIENTRY DllMain( HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
    )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

SMQ_EXTERN  smq_errno   smq_shm_open(smq_char* name, smq_uint32 size, smq_shm_t* shm)
{
    /// �����ɹ���shm->addr���벻ΪNULL���Ա���smq_shm_close�ܹ���ȷ��ִ���ͷŲ���
    return  SMQ_OK;
}

SMQ_EXTERN  smq_void    smq_shm_close(smq_shm_t* shm)
{
    SMQ_ASSERT((NULL != shm), "ָ��������ⲿ��֤");

    if (NULL == shm->addr)
    {
        return;
    }
}

SMQ_EXTERN  smq_errno   smq_proc_mutex_open(smq_char* name, smq_proc_mutex_t* mutex)
{
    return  SMQ_OK;
}

SMQ_EXTERN  smq_errno   smq_proc_mutex_lock(smq_proc_mutex_t* mutex)
{
    return  SMQ_OK;
}

SMQ_EXTERN  smq_errno   smq_proc_mutex_unlock(smq_proc_mutex_t* mutex)
{
    return  SMQ_OK;
}

SMQ_EXTERN  smq_void    smq_proc_mutex_close(smq_proc_mutex_t* mutex)
{
    return;
}

SMQ_EXTERN  smq_uint32  smq_proc_getpid()
{
    return  11;
}

